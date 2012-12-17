#include "mrmatch.h"
#include "Options.h"
#include <getopt.h>

#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Utils/FileUtils.h"

using namespace minerule;

namespace mrmatch {
	void Options::printUsage(int argc, char* const argv[]) {
		char* progName = argv[0];
		
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "  " << StringUtils::toBold(progName) << " [-n <num query>] [-O <optionlist>] [-v] [-h] [-s <sort order>] [<query name>] " << std::endl << std::endl;
	
		std::cout
			<< StringUtils::toBold("   -h") << " - prints this message." << std::endl
			<< StringUtils::toBold("   -c") << " - suppress colors."
			<< StringUtils::toBold("   -n") << " - specifies a query number (instead of a query name)." << std::endl
			<< StringUtils::toBold("   -O") << " - specifies a minerule option on the command line (overrides those read from file)." << std::endl
			<< StringUtils::toBold("   -f") << " - specify a file name containing the Options to be used." << std::endl
			<< "         default is 'optins.txt'"<< std::endl
			<< StringUtils::toBold("   -s") << " - sets the output sorting order. The parameter can be set to:" << std::endl
			<< StringUtils::toBold("         RuleGids") << ", to produce an output with the format: 'rule -> list of matching gids'" << std::endl
			<< StringUtils::toBold("         GidRules") << ", to produce an output with the format: 'gid -> list of matching rules'."  << std::endl
			<< "         default is 'RuleGids'. You can shorten the parameters as you like (e.g., 'G' for GidRules)." << std::endl
			<< StringUtils::toBold("   -v") << " -- version informations" << std::endl
			<< StringUtils::toBold("   -h") << " -- this message" << std::endl;
  
		exit(SUCCESS);
	}
	
	
	MatchKind Options::stringToMatchKind(const std::string& str) {
		if(std::string("RuleGids").find(str) == 0) // if str is a prefix for RuleGids
			return RuleGids;
		else if(std::string("GidRules").find(str) == 0) // if str is a prefix for GidRules
			return GidRules;
		else {
			std::cout << "Option parsing error: option -s can take only values in {RuleGids,GidRules}" << std::endl;
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
	}
	

	bool Options::initMineruleOptions() const {
		bool ok=false;
		MineruleOptions& options = MineruleOptions::getSharedOptions();
	
		if( !_mrOptionsFileName.empty() ) {
			options.readFromFile(_mrOptionsFileName);
			ok=true;
		} else if( FileUtils::fileExists("options.txt") ) {
			options.readFromFile("options.txt");
			ok=true;
		}
	
		std::vector<std::string>::const_iterator it;
		for(it=_mrOptionsInline.begin(); it!=_mrOptionsInline.end(); ++it) {
			options.readFromString(*it);
			ok=true;
		}
	
		return ok;
	}
	
	std::string Options::queryName() const {
		if( _queryNumber != 0 && !_queryName.empty() ) {
			std::cout << StringUtils::toBold("Error:") << "It appears that you specified both a query name and a query number" << std::endl;
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
		
		if( _queryNumber != 0 )
			return OptimizerCatalogue::getMRQueryName(_queryNumber);		
		
		if( !_queryName.empty() )
			return _queryName;
		
		std::cout << StringUtils::toBold("Error:") << "It appears that you specified neither a query name nor a query number" << std::endl;
		exit(MRMATCH_OPTION_PARSING_ERROR);
	}
	
	
	void Options::parse(int argc,  char* const argv[]) {
		char ch;
		
		while( (ch=getopt(argc, argv, "hcn:s:")) != -1 ) {
			switch(ch) {
				case 'h':
					printUsage(argc, argv);
					break;
				case 'c':
					StringUtils::setColorsEnabled(false);
					break;
				case 'n':
					setQueryNumber(Converter(optarg).toLong());
					break;
				case 's':
					setMatchKind(stringToMatchKind(optarg));
					break;
				case '?':
				default:
					std::cout << StringUtils::toBold("Option not recognized:") << "-" << ch << std::endl;
					printUsage(argc, argv);
					exit(MRMATCH_OPTION_PARSING_ERROR);
			}
		}
	
		if( optind < argc-1 ) {
			std::cout << StringUtils::toBold("Error:") << "some of the options were not recognized" << std::endl;
			
			printUsage(argc, argv);
			exit(MRMATCH_OPTION_PARSING_ERROR);
		}
		
		if( optind == argc-1 )
			setQueryName(argv[argc-1]);
	}
	

	

}
