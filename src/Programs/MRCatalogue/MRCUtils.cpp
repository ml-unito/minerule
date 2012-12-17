#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>
#include <string>
#include <algorithm>
#include <getopt.h>

#include "MRCUtils.h"
#include "Printer.h"
#include "minerule/Utils/FileUtils.h"
#include "minerule/Utils/StringUtils.h"


namespace mrc {
	// --------------------------------------------------------------------------------
	// OPTION HANDLING 
	// --------------------------------------------------------------------------------
	
	void doLoadOptions(const char* fname, minerule::MineruleOptions& opt) {
  
		if( minerule::FileUtils::fileExists(fname) ) {
			opt.readFromFile(fname);
		} else {
			std::string errstr;
			if(errno==0) {
				errstr = "Not a regular file!";
			} else {
				errstr = strerror(errno);
			}
    
			throw Exception(mrc::ERROR_CANNOT_OPEN_OPTION_FILE,
				std::string("Could not open file:") + fname + ". The reason is:" + errstr);
		}
	}

	void parseOptions(int argc, char** argv, minerule::MineruleOptions& mopt, Options& popt) { 
		const char* optstr = "cCIUhf:lF:n:d:v";
		
		int opt;
		bool didLoadOptions=false;
		while((opt=getopt(argc, argv, optstr))!=-1) {
			switch( opt ) {
			case 'c':
				minerule::StringUtils::setColorsEnabled(false);
				break;
			case 'C':
				popt.setCheckCatalogue();
				break;
			case 'U':
				popt.setUninstallCatalogue();
				break;
			case 'I':
				popt.setInstallCatalogue();
				break;
			case 'f': 
				doLoadOptions(optarg, mopt);
				didLoadOptions=true;
				break;
			case 'h':
				printHelp(argc, argv);
				break;
			case 'F':
				popt.setListFormat(optarg);
				break;
			case 'l':
				popt.setShowList();
				break;
			case 'n':
				popt.setSearchQry();
				popt.setSearchParam(Options::QryName, optarg);
				break;
			case 'd':
				popt.setDeleteQry();
				popt.setSearchParam(Options::QryName, optarg);
				break;
			case 'v':
				printVersion();
				exit(0);
			case '?':
			default:
				throw Exception( mrc::ERROR_OPTION_PARSING, "Error while parsing options, use -h to show" " the help message" );
			}
		}
		if( !didLoadOptions  ) {
			if( !minerule::FileUtils::fileExists(minerule::MineruleOptions::DEFAULT_FILE_NAME) )
				throw Exception( mrc::ERROR_OPTION_PARSING,"You should specify at least one -f option or provide ./"+minerule::MineruleOptions::DEFAULT_FILE_NAME);
			else
				doLoadOptions(minerule::MineruleOptions::DEFAULT_FILE_NAME.c_str(), mopt);
		}
	}

	void printVersion() {
		std::cout << "MRCatalogue v:" << MR_VERSION << std::endl;
	}
	
	// --------------------------------------------------------------------------------
	// ACTIONS 
	// --------------------------------------------------------------------------------

	void printHelp(int argc, char** argv) {
		using namespace minerule;
		std::cout << StringUtils::toBold("Usage:") << std::endl
			<< "   " << StringUtils::toBold(argv[0]) << " [-v] [-h] [-f  <optionfile>] [-C] [-I] [-U] [-n <queryname>] [-l] [-d] [-c] [-F <formatSpecs>]" << std::endl
			<< std::endl << std::endl 
			<< "Handles the minerule catalogue." << std::endl
			<< std::endl
			<< StringUtils::toBold("Program Information") << std::endl
			<< StringUtils::toBold("-v") << " - Output the version message and exits." << std::endl
			<< StringUtils::toBold("-h") << " - Output this message and returns NOTHING_TO_DO." << std::endl 
			<< std::endl 
			<< StringUtils::toBold("Option Handling") << std::endl
			<< StringUtils::toBold("-f") << " - file name of the option file (if omitted the program will)" << std::endl
			<< "     look for a file named 'option.txt' in the current directory" << std::endl
			<< std::endl 
			<< StringUtils::toBold("Installation") << std::endl
			<< StringUtils::toBold("-C") << " - Checks if the optimizer catalogue is installed correctly and exits." <<std::endl
			<< StringUtils::toBold("-I") << " - Installs the optimizer catalogue and exits." <<std::endl
			<< StringUtils::toBold("-U") << " - Uninstall the catalogue (be careful, this cannot be undone!). dbms must be either 'mysql' or 'postgres'." << std::endl
			<< std::endl 
			<< StringUtils::toBold("Dealing with Catalogue Entries") << std::endl
			<< StringUtils::toBold("-n") << " - Look in the catalogue for the specified query." << std::endl
			<< "     it returns QUERY_NAME_FOUND, or QUERY_NAME_NOT_FOUND" << std::endl
			<< "     accordingly to whether the query could be found."<<std::endl 
			<< StringUtils::toBold("-l") << " - Print the list of already executed queries, returns" << std::endl
			<< "     SUCCESS upon completion." << std::endl
			<< StringUtils::toBold("-d") << " - Delete the given minerule from the system (notice that" << std::endl
			<< "     the safety options in the option file must be setted" << std::endl
			<< "     in such a way to allow the deletion. " << std::endl
			<< std::endl 
			<< StringUtils::toBold("Output Handling") << std::endl				
			<< StringUtils::toBold("-c") << " - Disables color output." << std::endl
			<< StringUtils::toBold("-F") << " - Format specifiers for printing the list of queries (-l)" <<std::endl
			<< "     Valid specifiers: s - Print the size of the result set" << std::endl
			<< "                       t - Print the text of the original mr" <<std::endl
			<< "                       r - Print the result set table names" << std::endl
			<< "     the default is '', i.e.: print onlty the qry name." << std::endl
			<< std::endl 
			<< StringUtils::toBold("Return Values:") << std::endl
			<< "      SUCCESS = " << mrc::SUCCESS << std::endl
			<< "      NOTHING_TO_DO = " << mrc::NOTHING_TO_DO << std::endl
			<< "      QUERY_NAME_FOUND = " << mrc::QUERY_NAME_FOUND << std::endl
			<< "      CATALOGUE_NOT_INSTALLED = " << mrc::CATALOGUE_NOT_INSTALLED << std::endl
			<< "      QUERY_NAME_NOT_FOUND = " << mrc::QUERY_NAME_NOT_FOUND << std::endl
			<< "      ERROR_OPTION_PARSING = " << mrc::ERROR_OPTION_PARSING << std::endl
																																<< "      ERROR_CANNOT_OPEN_OPTION_FILE = " << mrc::ERROR_CANNOT_OPEN_OPTION_FILE <<std::endl;
	}
	
	int checkQueryName(const Options& options) {
		const std::string& tablename = 
			options.getSearchParam(Options::QryName);

		try {
			Printer printer(std::cout, options);
			minerule::CatalogueInfo info;
			minerule::OptimizerCatalogue::getMRQueryInfo(tablename,info, options.getListFormat().size);
			printer.print(info);
		} catch (minerule::MineruleException& mr) {
			std::cout << "The query you specified is NOT present in the catalogue." << std::endl;
			return mrc::QUERY_NAME_NOT_FOUND;
		}

		return mrc::QUERY_NAME_FOUND;
	}

	void printMRQueryList(const Options& options) {
		Printer printer(std::cout, options);
		std::vector<minerule::CatalogueInfo> mrqlist;
		minerule::OptimizerCatalogue::getMRQueryInfos(mrqlist, options.getListFormat().size );
		
		printer.print(mrqlist);
	}

	void deleteQuery(const Options& options) {
		if( checkQueryName(options)==mrc::QUERY_NAME_FOUND )
			minerule::OptimizerCatalogue::deleteMinerule( options.getSearchParam(Options::QryName) );
	}

	Results checkCatalogue()  {
		if(minerule::OptimizerCatalogue::checkInstallation()) {
			minerule::MRLog() << "The catalogue appears to be installed properly." << std::endl;
			return SUCCESS;					
		} else {
			minerule::MRLog() << "The catalogue is " << minerule::StringUtils::toRed("NOT") <<" installed" << std::endl;
			exit(CATALOGUE_NOT_INSTALLED);
		}		
	}
	
	Results installCatalogue() {
		minerule::MRLog() << "Checking current installation status." << std::endl;
		if(minerule::OptimizerCatalogue::checkInstallation()) {
			minerule::MRLog() << minerule::StringUtils::toBold("Minerule catalogue seems already installed.") << std::endl;
			minerule::MRLog() << minerule::StringUtils::toRed("Cowardly refusing to install over a working catalogue...") << std::endl;
			return NOTHING_TO_DO;
		} else {
			minerule::MRLog() << "Checking if everything is ok." << std::endl;
			minerule::MRLog() << minerule::StringUtils::toBold("The catalogue is not installed (properly).") << std::endl;
			minerule::MRLog() << "Proceeding with the installation..." << std::endl;
			minerule::OptimizerCatalogue::install();
			minerule::MRLog() << "Done!" << std::endl;
			minerule::MRLog() << "Checking the installation..." << std::endl;			
			if(!minerule::OptimizerCatalogue::checkInstallation()) {
				minerule::MRLog() << "Something went wrong. This is likely a bug. Please report it!" << std::endl;
				return CATALOGUE_NOT_INSTALLED;
			} else {
				minerule::MRLog() << minerule::StringUtils::toBold("The catalogue is now properly installed.") << std::endl;				
				return SUCCESS;
			}
		}
	}
	
	Results uninstallCatalogue() {
		minerule::MRLog() << "You choose to " << minerule::StringUtils::toBold(" uninstall ") << "the minerule catalogue" << std::endl;
		minerule::MRLog() << "This implies " << minerule::StringUtils::toBold(" destroying ") << "all its tables" << std::endl;

		std::cout << "Are you " << minerule::StringUtils::toBold("sure ") << "you want to proceed? (y/N): ";
		std::string confirm;
		std::cin >> confirm;
		transform(confirm.begin(), confirm.end(), confirm.begin(), ::toupper);
		if( confirm != "Y" && confirm != "YES") {
			minerule::MRLog() << "Ok. Bailing out." << std::endl;
			return NOTHING_TO_DO;
		}
		
		minerule::MRLog() << "Uninstalling the catalogue..." << std::endl;
		minerule::OptimizerCatalogue::uninstall();			
		minerule::MRLog() << "Done" << std::endl;
		
		return SUCCESS;
	}

	// --------------------------------------------------------------------------------
	// MAIN ACTION HANDLER 
	// --------------------------------------------------------------------------------
	
	int	performCommands(Options& options) {
		if( options.getCommand()==Options::ShowList ) {
			printMRQueryList(options);
			return mrc::SUCCESS;
		}

		if( options.getCommand()==Options::SearchQry) {
			return checkQueryName(options);
		}

		if( options.getCommand()==Options::DeleteQry ) {
			deleteQuery(options);
			return mrc::SUCCESS;
		}
		
		if( options.getCommand()==Options::CheckCatalogue ) {
			return checkCatalogue();
		}

		if( options.getCommand()==Options::InstallCatalogue ) {
			return installCatalogue();
		}

		if( options.getCommand()==Options::UninstallCatalogue ) {
			return uninstallCatalogue();
		}


		return mrc::NOTHING_TO_DO;
	}
} // namespace mrc
