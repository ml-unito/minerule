//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <iostream>
#include <unistd.h>
#include "minerule/Utils/MineruleOptions.hpp"


using namespace minerule;

void
printHelp(std::string progname) {
  std::cout << StringUtils::toBold("Usage: ") << progname << " [-h] [-v] [-f optionfile] [-O optionstring] [optfile]" << std::endl;
  std::cout << std::endl;
  std::cout 
	   << "  If invoked without arguments the program output (stdout)" << std::endl
       << "  the default options used by the Minerule system." << std::endl << std::endl
 	   << "  If " << StringUtils::toBold("-v") <<" is given, the program outputs its version number and exits" << std::endl <<std::endl
       << "  If " << StringUtils::toBold("-h") << " is given, the program outputs this message." << std::endl
       << "  If an option file is specified, the program reads it and" << std::endl
       << "  outputs the resulting options immediately afterwards" << std::endl
       << "  (useful to check that the file syntax is correct and that"<<std::endl
       << "  the program correctly parses it)." << std::endl
       << "  If more than one option file is specified, latter options" <<std::endl
       << "  override former ones. The same holds with the options" << std::endl
       << "  given using the "<<StringUtils::toBold("-O")<<" flag" << std::endl << std::endl;
  std::cout << StringUtils::toBold("NOTE:") << " all options regarding streams will be either commented"<< std::endl
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
      } catch( mrdb::SQLException& e) {
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

