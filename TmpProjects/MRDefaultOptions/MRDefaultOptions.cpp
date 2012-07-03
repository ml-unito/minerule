#include <iostream>
#include <unistd.h>
#include "Utils/MineruleOptions.h"

using namespace std;
using namespace minerule;

void
printHelp(string progname) {
  cout << "Usage: " << progname << " [-h] [-v] [-f optionfile] [-O optionstring] [optfile]" << endl;
  cout << "  If invoked without arguments the program output (stdout)" << endl
       << "  the default options used by the Minerule system." << endl << endl
       << "  If \"-h\" or if more than one argument is given, the"<< endl
       << "  program output this message." << endl
       << "  If an option file is specified, the program read it and" << endl
       << "  output the resulting options immediately afterwards" << endl
       << "  (useful to check that the file syntax is correct and that"<<endl
       << "  the program correctly parsify it)" << endl
       << "  If more than one option file is specified, the latter options" <<endl
       << "  override the former ones. The same holds with the options" << endl
       << "  given using the -O flag" << endl << endl;
  cout << "NOTE: all options regarding streams will be either commented"<< endl
       << "  or set to default values (for implementation reasons it is"<< endl
       << "  not possible to retrieve the file name they point to and " << endl
       << "  printing a number representing a pointer in memory is likely"<<endl
       << "  to lead to some kind of misunderstanding" <<endl;
}



void printVersion() {
  cout << "MRDefaults v:" << MR_VERSION << endl;
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
	mrOpts.readFromString(string(optarg));
      } catch( odbc::SQLException& e) {
	// do nothing
      }
      break;
    case 'f':
      mrOpts.readFromFile(string(optarg));
      break;
    default:
      cerr << "Option -" << opt 
	   << " not recognized! Please check the parameters" << endl;
      exit(1);
    }
  }

}


int
main(int argc, char** argv) {
  try {
    MineruleOptions& mropts = MineruleOptions::getSharedOptions();
    parseOptions(argc,argv,mropts);
    mropts.saveOptions(cout);
  } catch( std::exception& e) {
    cerr << e.what() << endl;
    exit(1);
  }
}

