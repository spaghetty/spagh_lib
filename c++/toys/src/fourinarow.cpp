#include <iostream>
#include <boost/program_options.hpp>

#include "4board.hpp"
#include "4player.hpp"

namespace po=boost::program_options;

enum RunningStatus { BadSyntax=1 };

struct Config {
  struct ParameterMissing { std::string name; };
  Config(int argn, const char **argv);

  void addMandatory(std::initializer_list<std::string> l);

  bool helpRequested(){
    return vm.count("help");
  }

  po::options_description desc{"Allowed options"};
  po::variables_map vm;
};

Config::Config(int argn, const char **argv) {
    desc.add_options()
      ("help,h", "produce help message")
      ("server,s", "work as server")
      ("player,p", po::value<std::string>()->default_value("base,base") ,"specify player type [base,ai]")
      ("client,c", po::value<std::string>()->default_value("127.0.0.1"), "work as client and connect to server specified as pi")
      ;
    po::store(po::parse_command_line(argn, argv, desc),vm);
    po::notify(vm);
};

void Config::addMandatory(std::initializer_list<std::string> l) {
    for(auto m: l) {
      if(!vm.count(m)){
	throw ParameterMissing{m};
      }
    }
};

int change_player(int i){
  if (i==0) {
    return 1;
  } else {
    return 0;
  }
} 

Player *selector(std::string s, const char c, std::string servip) {
  if(s=="base") {
    return new Player(c);
  } else if( s=="serv" ) {
    return new ServPlayer(c);
  } else if( s=="cli" ) {
    return new CliPlayer(c, servip);
  } else {
    return nullptr;
  }
}

bool allocate_players(Player *players[2], std::string schema, std::string servip) {
  std::string values[2] = {"", ""};
  auto tmp = schema.find(',',0);
  if (tmp==std::string::npos) {
    std::cout << "merd" << std::endl;
    tmp = schema.size();
  }
  std::string s = schema.substr(0,tmp);
  players[0] = selector(s, 'O', servip);
  if(players[0] == nullptr)
    return false;
  if(tmp<schema.size()){
    std::string s = schema.substr(tmp+1,schema.length());
    players[1] = selector(s, 'X', servip);
  } else {
    players[1] = new Player('X');
  }
  if(players[1]==nullptr)
    return false;
  return true;

}

int main(int argn, const char **argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  Config conf{argn, argv};
  try {
    //conf.addMandatory({"test"}); // add mandatory stuff 
  } catch (Config::ParameterMissing m) {
    std::cout << "Syntax error in command call, missing mandatory parameter: " << m.name << std::endl;
    return RunningStatus::BadSyntax;
  }
  if (conf.helpRequested()) {
    std::cout << conf.desc << std::endl;
  }

  Board board;
  Player *players[2] = {nullptr, nullptr};
  allocate_players(players,conf.vm["player"].as<std::string>(), conf.vm["client"].as<std::string>());
  std::cout << board.toString() << std::endl;
  auto i=0;
  auto exitstatus = board.status();
  while(exitstatus==Board::Status::Ready) {
    int place = players[i]->play(board);
    try {
      board.play(place, players[i]->sign);
    } catch(Board::Exception &e) {
      std::cout << "Error playing " << place << " " << e.name() << std::endl;
      i = change_player(i);
    }
    std::cout << board.toString() << std::endl;
    // std::string data;
    // board.toProtoBuf().SerializeToString(&data); // using protobuf for deck transfer
    // std::cout << data << std::endl;
    exitstatus=board.status();
    i = change_player(i);
  }
  if(exitstatus==Board::Status::Full) {
    std::cout << "Board is full match game" << std::endl;
  } else if(exitstatus==Board::Status::Win) {
    auto tmp = change_player(i);
    std::cout << "Player " << players[tmp]->sign << " Win" << std::endl;
  }
  google::protobuf::ShutdownProtobufLibrary();
  if(players[0]!=nullptr) 
    delete players[0];
  if(players[1]!=nullptr)
    delete players[1];
  return RunningStatus{};
}
