#include <iostream>
#include <unistd.h>
#include "Utils/MineruleOptions.h"


using namespace minerule;

void
printHelp(std::string progname) {
  std::cout << StringUtils::to_bold("Usage: ") << progname << " [-h] [-v] [-f optionfile] [-O optionstring] [optfile]" << std::endl;
  std::cout << std::endl;
  std::cout 
	   << "  If invoked without arguments the program output (stdout)" << std::endl
       << "  the default options used by the Minerule system." << std::endl << std::endl
 	   << "  If " << StringUtils::to_bold("-v") <<" is given, the program outputs its version number and exits" << std::endl <<std::endl
       << "  If " << StringUtils::to_bold("-h") << " is given, the program outputs this message." << std::endl
       << "  If an option file is specified, the program reads it and" << std::endl
       << "  outputs the resulting options immediately afterwards" << std::endl
       << "  (useful to check that the file syntax is correct and that"<<std::endl
       << "  the program correctly parses it)." << std::endl
       << "  If more than one option file is specified, latter options" <<std::endl
       << "  override former ones. The same holds with the options" << std::endl
       << "  given using the "<<StringUtils::to_bold("-O")<<" flag" << std::endl << std::endl;
  std::cout << StringUtils::to_bold("NOTE:") << " all options regarding streams will be either commented"<< std::endl
       << "  or set to default values (for implementation reasons it is"<< std::endl
       << "  not possible to retrieve the file name they point to and " << std::endl
       << "  printing a number representing a pointer in memory is likely"<<std::endl
       << "  to lead to some kind of misunderstanding)" <<std::endl;
}



void printVersion() {
  std::cout << "mrdefaults v:" << MR_VERSION << std::endl;
}

void
parseOptions(int argc, char** argv, MineruleOptions& mrOpts) {
  int opt;
  

  while((opt=getopt(argc,argv,"hf:O:v"))!=-1) {
    switch(opt) {
    case 'h':
      printHelp(argv[0]);
      exit(0);
    case 'v':
      printVersion();
      exit(0);
    case 'O':
      try {
	mrOpts.readFromString(std::string(optarg));
      } catch( odbc::SQLException& e) {
	// do nothing
      }
      break;
    case 'f':
      mrOpts.readFromFile(std::string(optarg));
      break;
    default:
      std::cerr << "Option -" << opt 
	   << " not recognized! Please check the parameters" << std::endl;
      exit(1);
    }
  }

}


int
main(int argc, char** argv) {
  try {
    MineruleOptions& mropts = MineruleOptions::getSharedOptions();
    parseOptions(argc,argv,mropts);
    mropts.saveOptions(std::cout);
  } catch( std::exception& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
}

