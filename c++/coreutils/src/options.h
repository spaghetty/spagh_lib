#include <boost/program_options.hpp>

namespace po=boost::program_options;

enum RunningStatus { BadSyntax=1, HelpRequest };

struct Config {
  struct ParameterMissing { std::string name; };

  void AddMandatory(std::initializer_list<std::string> l);
  void Parse(int argn, const char **argv);

  bool HelpRequested(){
    return vm.count("help");
  };

  po::positional_options_description positional;
  po::options_description desc{"Allowed options"};
  po::variables_map vm;
};

void Config::Parse(int argn, const char **argv) {
  //po::store(po::parse_command_line(argn, argv, desc),vm);
  po::store(po::command_line_parser(argn, argv).options(desc).positional(positional).run(),vm);
  po::notify(vm);
};

void Config::AddMandatory(std::initializer_list<std::string> l) {
  for(auto m: l) {
    if(!vm.count(m)){
      throw ParameterMissing{m};
    }
  }
};
