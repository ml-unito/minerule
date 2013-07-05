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
#include "Options.h"
#include <getopt.h>
#include <iostream>

#include "minerule/Utils/StringUtils.h"
#include "minerule/Utils/Converter.h"
#include "minerule/Utils/FileUtils.h"

namespace mrprint {
	
	void printError(const std::string& error) {
		std::cout << minerule::StringUtils::toRed("Error:") << error << std::endl;
	}
	
	void Options::printHelp() const {
		std::cout << minerule::StringUtils::toBold("Usage:") << std::endl
			<< "   " << minerule::StringUtils::toBold(_argv[0]) << " [-h] [-f <optionfile>] [-0] [-c] [-n minerule-number] [-s <order>] resultsetname" << std::endl
			<< "The program allows printing results of minerule queries." << std::endl 
			<< std::endl
			<< minerule::StringUtils::toBold("-h") << " - prints this message " << std::endl
			<< minerule::StringUtils::toBold("-f") << " - Specifies a minerule option file (default: '"<< minerule::MineruleOptions::DEFAULT_FILE_NAME << "')" << std::endl
			<< minerule::StringUtils::toBold("-c") << " - suppress colors " << std::endl
			<< minerule::StringUtils::toBold("-0") << " - suppresses logging artifacts " << std::endl
			<< minerule::StringUtils::toBold("-l") << " - do not filter out rules having low confidence" << std::endl	
			<< minerule::StringUtils::toBold("-n") << " - specifies a query number to be printed (this is an alternative to" << std::endl
			<< "     specifying the query name)" << std::endl
			<< minerule::StringUtils::toBold("-s") << " - sorts the rules in a specified order." << std::endl
			<< "   supported orders are: " << std::endl
			<< "     'no' -> no particular order (fastest display)" << std::endl
			<< "     'scbh' -> order is support, confidence, body, head"<<std::endl
			<< "     'bhsc' -> order is body, head, supp, conf"<<std::endl
			<< "     'hbsc' -> order is head, body, supp,  conf"<<std::endl
			<< "     'csbh' -> order is conf, supp, body, head" << std::endl
			<< "     'cbhs' -> order is conf, body, head, supp" << std::endl
			<< "     'cbsh' -> order is conf, body, supp, head" << std::endl
			<< "   the default is 'no'" << std::endl
			<< std::endl << std::endl;
	}
	
	void Options::parse() {
		const char* optstr = "hc0ln:s:";
		
		int opt;
		while( (opt = getopt(_argc, _argv, optstr)) != -1 ) {
			switch(opt) {
				case 'h':
				printHelp();
				break;
				case 'f':
				_mroptFileName = optarg;
				break;
				case 'c':				
				minerule::StringUtils::setColorsEnabled(false);
				break;
				case '0':
				_noLogArtifacts=true;
				break;
				case 'l':
				_noLowConfidenceFilter=true;
				break;
				case 'n':
				_queryNumber = minerule::Converter(optarg).toLong();
				break;
				case 's':
				_sortOrder = optarg;
				break;
				default:
				std::cout << minerule::StringUtils::toRed("Error:") << " option " << opt << " not recognized." << std::endl;
				exit(MRPRINT_OPTION_PARSING_ERROR);
			}
		}
			
		if(optind == _argc-1) {
			_queryName = _argv[optind];
		}
			
		bool noQueryName = _queryName.empty() && _queryNumber == 0;
		bool noOptionFile = !minerule::FileUtils::fileExists(_mroptFileName);
			
		if( noQueryName ) {
			printError("no query name has been given. Please give one or use -n to specify a query number.");
			exit(MRPRINT_OPTION_PARSING_ERROR);
		}
			
		if(noOptionFile) {
			printError("option file " + _mroptFileName + " not found.");
			exit(MRPRINT_OPTION_PARSING_ERROR);
		}
	}

}