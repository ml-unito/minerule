#include <iostream>
#include <getopt.h>

#include "Result/RulesMatcher.h"
#include "Options.h"

using namespace minerule;

namespace mrmatch {
	typedef enum { SUCCESS=0, MRMATCH_OPTION_PARSING_ERROR } ExitCodes;
	
	void printUsage(int argc, char* const argv[]) {
		char* progName = argv[0];
		
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "  " << StringUtils::toBold(progName) << " [-n <num query>] [-O <optionlist>] [-v] [-h] [<query name>] " << std::endl << std::endl;
	
		std::cout 
			<< StringUtils::toBold("   -n") << " -- specifies a query number (instead of a query name)" << std::endl
			<< StringUtils::toBold("   -O") << " -- specifies a minerule option on the command line (overrides those read from file)" << std::endl
			<< StringUtils::toBold("   -f") << " -- specify a file name containing the Options to be used." << std::endl
			<< "         default is 'optins.txt'"<< std::endl;
			// << StringUtils::toBold("   -v") << " -- version informations" << std::endl
			// << StringUtils::toBold("   -h") << " -- this message" << std::endl
  
		exit(SUCCESS);
	}

	Options parseOptions(int argc,  char* const argv[]) {
		char ch;
		Options options;
		
		while( (ch=getopt(argc, argv, "hn:")) != -1 ) {
			switch(ch) {
			case 'n':
				options.setQueryNumber(Converter(optarg).toLong());
				break;
			case '?':
			default:
				std::cout << StringUtils::toBold("Option not recognized:") << "-" << ch << endl;
				printUsage(argc, argv);
				exit(MRMATCH_OPTION_PARSING_ERROR);
			}
		}
	
		if( optind < argc-1 ) {
			cout << StringUtils::toBold("Error:") << "some of the options were not recognized" << std::endl;
			
			printUsage(argc, argv);
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
		
		if( optind == argc-1 )
			options.setQueryName(argv[argc-1]);
	
	    return options;
	}
}




int main (int argc, char* const argv[]) {
	mrmatch::Options options;
	
	options = mrmatch::parseOptions(argc, argv);
	if(!options.initMineruleOptions()) {
		std::cout << "Minerule Options not specified." << std::endl
			<< "They can be specified in several ways, you can:" << std::endl
			<< " provide the path to an option file using flag " << StringUtils::toBold("-f") << "." << std::endl
			<< " launch the program from a directory containing an option file named 'options.txt'" << std::endl	
			<< " provide the options on the command line using " << StringUtils::toBold("-O") << " flags." << std::endl;

		exit(mrmatch::MRMATCH_OPTION_PARSING_ERROR);
	}
	
	MRLog() << "query name:" << options.queryName() << std::endl;
	MRLog() << "query number:" << options.queryNumber() << std::endl;
	
	return 0;
}