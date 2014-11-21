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
#include "mrmatch.hpp"
#include "Options.hpp"
#include <getopt.h>

#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Utils/FileUtils.hpp"

using namespace minerule;

namespace mrmatch {
	void Options::printUsage(int argc, char* const argv[]) {
		char* progName = argv[0];
		
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "  " << StringUtils::toBold(progName) << " [-h] [-c] [-n <num query>] [-O <optionlist>] [-f <optionfile>] [-d <outputtable>]  [-t <table name>] [-s <sort order>] [<query name>] " << std::endl << std::endl;
	
		std::cout
			<< StringUtils::toBold("   -h") << " - prints this message." << std::endl
			<< StringUtils::toBold("   -c") << " - suppress colors." << std::endl
			<< StringUtils::toBold("   -n") << " - specifies a query number (instead of a query name)." << std::endl
			<< StringUtils::toBold("   -O") << " - specifies a minerule option on the command line (overrides those read from file)." << std::endl
			<< StringUtils::toBold("   -f") << " - specify a file name containing the Options to be used." << std::endl
			<< "         default is 'optins.txt'"<< std::endl
			<< StringUtils::toBold("   -d") << " - redirect the output onto the database. The output table must" <<std::endl
			<< "         be specified as the argument to this option" << std::endl
			<< StringUtils::toBold("   -t") << " - specify a table name for the match (the table *must* have the" << std::endl
			<< "         same schema as the mining table used for the query)" << std::endl
			<< StringUtils::toBold("   -s") << " - sets the output sorting order. The parameter can be set to:" << std::endl
			<< StringUtils::toBold("         RuleGids") << ", to produce an output with the format: 'rule -> list of matching gids'" << std::endl
			<< StringUtils::toBold("         GidRules") << ", to produce an output with the format: 'gid -> list of matching rules'."  << std::endl
			<< "         default is 'RuleGids'. You can shorten the parameters as you like (e.g., 'G' for GidRules)." << std::endl;
  
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
	
	const std::string& Options::tableName() const {
		return _tableName;
	}
	
	void Options::setTableName(const std::string& tableName) {
		_tableName = tableName;
	}
	
	
	void Options::parse(int argc,  char* const argv[]) {
		char ch;
		
		while( (ch=getopt(argc, argv, "hcn:s:d:t:")) != -1 ) {
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
				case 'd':
					setMatchOutputTableName(optarg);
					setMatchOutput(OutOnDB);
					break;
				case 't':
					setTableName(optarg);
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
