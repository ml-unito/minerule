#include <iostream>
#include <getopt.h>

#include "Result/RulesMatcher.h"
#include "Options.h"

using namespace minerule;

namespace mrmatch {
	typedef enum { SUCCESS=0, MRMATCH_OPTION_PARSING_ERROR } ExitCodes;

	Options parseOptions(int argc, const char* argv[]) {
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
	
		if( optind > argc-1 ) {
			printUsage(argc, argv);
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
	
	    return options;
	}
}


int main (int argc, char const *argv[]) {
	mrmatch::Options options;
	
	options = mrmatch::parseOptions(argc, argv);
	if(!options.loadMineruleOptions()) {
		std::cout << "Please specify minerule options parameters." << std::endl
			<< "Your choices are:" << std::endl
			<< " provide a" << StringUtils("-f") << " option." << std::endl
			<< " launch the program from a directory containing an option file named options.txt" << std::endl	
			<< " provide the options on the command line using " << StringUtils("-O") << " options." << std::endl;

		exit(MRMATCH_OPTION_PARSING_ERROR);
	}
	
	MRLog() << "query name:" << options.queryName() << std::endl;
	MRLog() << "query number:" << options.queryNumber() << std::endl;
	
	return 0;
}