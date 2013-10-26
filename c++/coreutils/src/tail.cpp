#include <iostream>
#include <fstream>

#include "options.h"

const int MAXBUF=1024;

class BaseFile {
public:
  BaseFile(std::ifstream &f, Config &conf):file(f),file_position(0),c(conf) {
    f.seekg(0,file.end);
    file_size = file.tellg();
    if(file_size>MAXBUF) {
      file_position = file_size-MAXBUF;
    }
    linecount=c.vm["line"].as<int>();
  };

  ~BaseFile() {
    file.close();
  };
  
  void Run() {
    int tot_len=0;
    bool valid=true;
    while(true) {
      file.seekg(file_position);
      int length = file.readsome(buf,MAXBUF);
      int i = length;
      // count a line if line don't ends with newline
      for(; i>0 && linecount>0; --i) {
	if(buf[i]=='\n') {
	  --linecount;
	}
      }
      ++i;
      tot_len+=(length-i);
      if(linecount==0){
	if(valid)
	  write_out(i, tot_len, valid);
	else 
	  write_out(file_size-tot_len, tot_len, valid);
	break;
      } else if(length>=file_size) {
	write_out(0,file_size, valid);
	break;
      }
      file_position-=MAXBUF;
      valid=false;
    }
  };

private:
  void write_out(int start, int length, bool valid=false) {
    if( valid ) {
      std::cout.write(buf+start,length);
    } else {
      char b[length];
      file.seekg(start);
      int l = file.readsome(b,length);
      std::cout.write(b,length);
    }
  }


  char buf[MAXBUF];
  std::ifstream &file;
  int file_size;
  int file_position;
  int linecount;
  Config &c;
};


int main(int argn, const char **argv) {
  Config c;
  try {
  c.desc.add_options()
    ("help,h", "produce help message")
    ("line,n", po::value<int>()->default_value(10), "Binding address")
    ("file,f", po::value<std::string>(),"Binding port")
    ;
  c.positional.add("file",1);
  c.Parse(argn, argv);
  c.AddMandatory({"file"});
  } catch (Config::ParameterMissing m) {
    std::cout << "Syntax error in command call, missing mandatory parameter: " << m.name << std::endl;
    return RunningStatus::BadSyntax;
  }
  std::cout << c.vm["file"].as<std::string>() << std::endl;
  
  std::ifstream f(c.vm["file"].as<std::string>(),std::ifstream::binary);
  if(f.is_open()) {
    BaseFile myf{f,c};
    
    myf.Run();
    //std::cout << "(" << length << ")"<< i << ":" << linecount << std::endl;
    return 0;
  }
  return -1;
}
