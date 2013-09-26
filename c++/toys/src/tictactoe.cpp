#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <v8.h>
#include <sys/stat.h>
#include <boost/program_options.hpp>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/DynamicAny.h>

#define MAX 3
#define EMPTY 95

namespace po=boost::program_options;

std::string aiFileName = "aiplayer";

typedef struct move_t {
  int x;
  int y;
} move;

int weight_evaluator(bool ismine, int oldval) {
  if(ismine) {
    switch (oldval) {
    case -1000:
      return 1;
    case 1:
      return 10;
    case 10:
      return 100;
    case -1:
      return 0; // i just think that this cases are hard
    case -10:
      return 0;
    case -100:
      return 0;
    case 0:
      return 0;
    }
  } else {
    switch (oldval) {
    case -1000:
      return -1;
    case -1:
      return -10;
    case -10:
      return -100;
    case 1:
      return 0;
    case 10:
      return 0;
    case 100:
      return 0;
    case 0:
      return 0;
      
    }
  }
};

class Desk {
private:
  char desk[MAX][MAX];
  int freeSpace;

public:
  Desk() {
    for(auto i=0; i<MAX; i++){
      for(auto j=0; j<MAX; j++) {
	desk[i][j]=EMPTY;
      }
    }
    freeSpace = MAX*MAX;
  }

  Desk(std::string &l){
    int j=0;
    for(auto i=l.begin(); i<l.end(); ++i) {
      desk[j/MAX][j%MAX] = (*i);
      if ((*i)!=EMPTY)
	freeSpace--;
      j++;
    }
  }
  
  bool set(char c, int x, int y) {
    if(x>=MAX || y>=MAX){
      return false;
    }
    if(desk[x][y]!=EMPTY) {
      return false;
    }
    desk[x][y] = c;
    freeSpace--;
    return true;
  }

  char get(int x, int y) const {
    return desk[x][y];
  }

  void print() const {
    for ( auto i=0; i<=MAX; i++ ) {
      if(i!=MAX) {
	std::cout << i+1 << ") ";
	std::cout << desk[i][0]; 
	for( auto j=1; j < MAX; j++) {
	  std::cout << "|"<< desk[i][j];
	}
	std::cout << std::endl;
      } else {
	std::cout << "   1 2 3" << std::endl;
      }
    }
  }

  bool checkStatus(const char c, std::array<int,8> &values) const {
    for(auto i=0; i<MAX; i++) {
      for(auto j=0; j<MAX; j++) {
  	if(get(i,j)!= EMPTY) {
	  bool is_mine= (get(i,j)==c);
  	  values[i] = weight_evaluator(is_mine,values[i]);
  	  values[MAX+j] = weight_evaluator(is_mine,values[MAX+j]);
  	  if(i==j) {
  	    values[6] = weight_evaluator(is_mine,values[6]);
  	  }
  	  if(i+j==MAX-1) {
  	    values[7] = weight_evaluator(is_mine,values[7]);
  	  }
  	}
      }
    }
    return true;
  }

  std::string *serialize() const {
    auto v = new std::string();
    for(auto i=0; i<MAX; i++){
      for(auto j=0; j<MAX; j++) {
	v->push_back(desk[i][j]);
      }
    }
    return v;
  }

  bool has_space() const {
    return (freeSpace>0);
  }
};

class Player {
protected:
  char mysign;
public:
  Player(const char c):mysign(c) {};
  char get_sign() const { return mysign; };
  virtual void play(Desk *d) {};
  virtual void response(int status) const {};
};

class BasePlayer: public Player {
public:
  BasePlayer(const char c):Player(c){};
  virtual void play(Desk *d) {
    int x=0, y=0;
    while(true) {
      while( !(y>0 && y<MAX+1 && x>0 && x<MAX+1)) {
	std::cout << get_sign() <<") ";
	std::cout << "Inserisci x e y: ";
	if(!(std::cin >> y >> x)){ //hard way to check input 
	  std::cin.clear();
	  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
      }
      if(!d->set(get_sign(), x-1, y-1)){
	x = 0;
	y = 0;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "error inserting element (" << x-1 << " " << y-1 << ") replay" << std::endl;
	continue;
      }
      break;
    }
  }
};

class NetPlayer: public Player {
  Poco::Net::ServerSocket srv;
  Poco::Net::StreamSocket ss;
  Poco::Net::SocketStream *str;
public:
  NetPlayer(const char c):Player(c),srv(5099) {
    std::cout << "Waiting for client connection..." << std::flush;
    ss = srv.acceptConnection();
    ss.setNoDelay(true);
    str = new Poco::Net::SocketStream(ss);
    std::cout << "got from : " << ss.address().toString() << std::endl;
    
  }
  
  virtual void play(Desk *d) {
    auto v = d->serialize();
    int x,y;
    (*str) << (*v) << std::endl << std::flush;
    (*str) >> x >> y;
    while(true) {
      while( !(y>0 && y<MAX+1 && x>0 && x<MAX+1)) {
	(*str) >> x >> y;
      }
      if(!d->set(get_sign(), x-1, y-1)){
	x = 0;
	y = 0;
	std::cout << "error inserting element (" << x-1 << " " << y-1 << ") replay" << std::endl;
	continue;
      }
      break;
    }    
    std::cout << ":" << x << y << std::endl;
  }
  virtual void response(int res) const {
    (*str) << res << std::endl << std::flush;
  }
};

class DynPlayer: public Player {
  
  v8::Handle<v8::Script> script;
  std::string content;

public:
  DynPlayer(const char c): Player(c) {
    std::string myname = aiFileName + ".js";
    struct stat buffer;
    if (stat(myname.c_str(),&buffer)!=0) {
      myname = aiFileName + c + ".js";
    }
    std::ifstream in(myname.c_str(), std::ifstream::in);
    
    if (in) {
      in.seekg(0,std::ios::end);
      content.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&content[0], content.size());
      in.close();
    }
    //v8::Isolate* isolate = v8::Isolate::GetCurrent();
    //v8::HandleScope handle_scope(isolate);
    //context = v8::Context::New(isolate);
    //v8::Context::Scope context_scope(context);
    //v8::Handle<v8::String> source = v8::String::New(content.c_str());
    //script = v8::Script::Compile(source);

  }

  ~DynPlayer(){};
  
  virtual void play(Desk *d) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handle_scope(isolate);
    v8::Handle<v8::Context> context = v8::Context::New(isolate);
    v8::Context::Scope context_scope(context);
    v8::Handle<v8::Array> deskr1 = v8::Array::New(3);
    v8::Handle<v8::Array> deskr2 = v8::Array::New(3);
    v8::Handle<v8::Array> deskr3 = v8::Array::New(3);
    v8::Handle<v8::Array> desk = v8::Array::New(3);
    deskr1->Set(0,v8::Integer::New(d->get(0,0)));
    deskr1->Set(1,v8::Integer::New(d->get(0,1)));
    deskr1->Set(2,v8::Integer::New(d->get(0,2)));
    deskr2->Set(0,v8::Integer::New(d->get(1,0)));
    deskr2->Set(1,v8::Integer::New(d->get(1,1)));
    deskr2->Set(2,v8::Integer::New(d->get(1,2)));
    deskr3->Set(0,v8::Integer::New(d->get(2,0)));
    deskr3->Set(1,v8::Integer::New(d->get(2,1)));
    deskr3->Set(2,v8::Integer::New(d->get(2,2)));
    desk->Set(0,deskr1);
    desk->Set(1,deskr2);
    desk->Set(2,deskr3);
    context->Global()->Set(v8::String::New("desk"),desk, v8::ReadOnly);
    v8::Handle<v8::Integer> sign = v8::Integer::New(mysign);
    context->Global()->Set(v8::String::New("_MYSIGN__"),sign, v8::ReadOnly);
    v8::Handle<v8::Integer> empty = v8::Integer::New(EMPTY);
    context->Global()->Set(v8::String::New("_EMPTY_"),empty, v8::ReadOnly);
    v8::Handle<v8::String> source = v8::String::New(content.c_str());
    script = v8::Script::Compile(source);
    v8::Handle<v8::Value> result = script->Run();
    
    v8::Handle<v8::Array> val = v8::Handle<v8::Array>::Cast(result);

    v8::Local<v8::Integer> vX = v8::Local<v8::Integer>::Cast(val->Get(0));
    v8::Local<v8::Integer> vY = v8::Local<v8::Integer>::Cast(val->Get(1));
    std::cout << vY->Value()+1 << "-"<< vX->Value()+1 << std::endl;
    bool res = d->set(get_sign(), vX->Value(), vY->Value());
    assert(res==true);
  }

};

class AIPlayer : public Player {
  std::function<int(void)> entropy;

public:
  AIPlayer(const char c):Player(c) {
    std::random_device rdev{};
    std::mt19937 generator(rdev());
    std::uniform_int_distribution<int> distribution(1,10);
    entropy = std::bind ( distribution, generator );
  };

  ~AIPlayer() {
    //delete (entropy->target());
  };

  virtual void play(Desk *d) {
    std::vector<move*> *t = generate_moves(d);
    move *m = select_move(d,*t);
    d->set(get_sign(), m->x, m->y);
      //std::cout << "fuck error" << std::endl;
    delete t;
  }
private:
  std::vector<move*> *generate_moves(Desk *d){
    std::vector<move*> *tmp = new std::vector<move*>();
    for(int i=0; i<MAX; i++) {
      for(int j=0; j<MAX; j++) {
	if (d->get(i,j)==EMPTY) {
	  auto t = new move{i,j};
	  tmp->push_back(t);
	}
      }
    }
    return tmp;
  }
  
  int compute_result(move *i, std::array<int,8> values){
    std::array<int,8> tmp = values;
    tmp[i->x] = weight_evaluator(true, tmp[i->x]);
    tmp[MAX+i->y] = weight_evaluator(true, tmp[MAX+i->y]);
    if(i->x==i->y) {
      tmp[6] = weight_evaluator(true, tmp[6]);
    }
    if((i->x+i->y)==MAX-1) {
      tmp[7] = weight_evaluator(true,tmp[7]);
    }
    for(auto j=0; j<8; j++) {
      if(tmp[j] == -1000) {
	tmp[j] = 0;
      }
    }
    std::cout << i->x << " " << i->y << ")";
    for( int i = 0; i<8; i++) {
      std::cout << tmp[i] << " ";
    }
    std::cout << std::endl;
    int sum=0;
    for(int k = 0; k<8; k++) {
      sum += tmp[k];
    }
    return sum;
  }


  move *select_move(Desk *d, std::vector<move*> &m) {
    std::array<int,8> values = { -1000, -1000, -1000, -1000,
				 -1000, -1000, -1000, -1000};

    d->checkStatus(mysign,values);

    int f_sum = -1000;
    move *mymove = m[0];

    for( auto i = m.begin(); i < m.end(); ++i) {
      auto tmp_sum = compute_result(*i,values);
      if(tmp_sum > f_sum) {
	f_sum = tmp_sum;
	mymove = *i;
      } else if(tmp_sum == f_sum) {
	// choose randomly between same level moves
	auto e = entropy();
	if (e > 5) {
	  f_sum = tmp_sum;
	  mymove = *i;
	}
      }
    }
    return mymove;
  }
};

class Arbiter {
  Player *first;
  Player *second;

public:
  Arbiter(Player *a, Player *b):first(a),second(b) {}

  int checkStatus(Desk *d) {
    std::array<int,8> values =  { -1000, -1000, -1000, -1000,
				 -1000, -1000, -1000, -1000};

    char mysign(first->get_sign());
    d->checkStatus(mysign, values);
    for(auto j=0; j<8; j++) {
      if(values[j] == -1000) {
	values[j] = 0;
      }
    }
    int rt=0;
    for(int k=0; k < 8; k++) {
      if(values[k]==-100){
	rt = -1;
      }else if(values[k]==100){
	rt = 1;
      }
    }
    return rt;
  }
};

Player *selector(std::string s, const char c) {
  if(s=="base") {
    return new BasePlayer(c);
  } else if (s=="ai") {
    return new AIPlayer(c);
  } else if (s=="net") {
    return new NetPlayer(c);
  } else if (s=="dyn") {
    return new DynPlayer(c);
  } else {
    return nullptr;
  }
}

bool allocate_player(Player *v[2], std::string schema) {
  std::string values[2] = {"", ""};
  auto tmp = schema.find(',',0);
  if (tmp==std::string::npos) {
    std::cout << "merd" << std::endl;
    tmp = schema.size();
  }
  std::string s = schema.substr(0,tmp);
  v[0] = selector(s, 'O');
  if(v[0] == nullptr)
    return false;
  if(tmp<schema.size()){
    std::string s = schema.substr(tmp+1,schema.length());
    v[1] = selector(s, 'X');
  } else {
    v[1] = new BasePlayer('X');
  }
  if(v[1]==nullptr) 
    return false;
  return true;
}

int main(int argn, char **argv) {
  po::options_description desc("Allowed options");
  po::variables_map vm;
  desc.add_options()
    ("help", "produce help message")
    ("player,p", po::value<std::string>()->default_value("base,ai") ,"specify player type [base,ai]")
    ("client,c", "use as client someone other shuld have run the program with remote player")
    ("ip",po::value<std::string>()->default_value("127.0.0.1"), "server ip, valid just with client option enabled")
    ;
  po::store(po::parse_command_line(argn, argv, desc), vm);
  po::notify(vm);
  if (vm.count("help")) {
    std::cout << desc << "\n";
    return 1;
  }
  if (vm.count("client")) {
    std::cout << "client starts here" << std::endl;
    std::string ip = vm["ip"].as<std::string>();
    Poco::Net::SocketAddress sa(ip, 5099);
    Poco::Net::StreamSocket socket(sa);
    Poco::Net::SocketStream str(socket);
    Player *p = new BasePlayer('Y');
    int response=0;
    for(;;) {
      std::string status;
      str >> status;
      if(status.size()!=9) 
	continue;
      auto d = new Desk(status);
      d->print();
      //std::cout << status << std::endl;
      int x=0, y=0;
      while( !(y>0 && y<MAX+1 && x>0 && x<MAX+1)) {
	std::cout << p->get_sign() <<") ";
	std::cout << "Inserisci x e y: ";
	if(!(std::cin >> y >> x)){ //hard way to check input 
	  std::cin.clear();
	  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      str << x << std::endl << y << std::endl << std::flush;
      str >> response;
      str.flush();
      if(response>0) {
	std::cout << "You Win" << std::endl;
	break;
      }
      if(response<0) {
	std::cout << "You Loose" << std::endl;
	break;
      } 
    }

  } else {
    auto d = new Desk();
    Player *a[2] = {};
    bool turn=true;
    int tmp = 0;
    if(allocate_player(a, vm["player"].as<std::string>())) {
      Arbiter arbiter(a[0],a[1]);
      d->print();
      while (d->has_space()) {
	if(turn) {
	  tmp = 0;
	  turn = !turn;
	} else {
	  tmp = 1;
	  turn = !turn;
	}
	a[tmp]->play(d);
	d->print();
	auto result = arbiter.checkStatus(d);
	a[0]->response(result);
	a[1]->response(result*-1);
	if(result > 0) {
	  std::cout << "Player " << a[0]->get_sign() << " win" << std::endl;
	  break;
	}
	if(result < 0) {
	  std::cout << "Player " << a[1]->get_sign() << " win" << std::endl;
	  break;
	}
      }
      delete a[0];
      delete a[1];
    }
    delete d;
  }

}
