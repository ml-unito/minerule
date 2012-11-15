#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<iterator>
#include<cstring>
#include<cctype>

#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include "Parsers/ParsedMinerule.h"
#include "Algorithms/Algorithms.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/MineruleOptions.h"
#include "Utils/MineruleErrors.h"
#include "Utils/FileUtils.h"



using namespace minerule;


void
printHelp(char* progName) {
	std::cout << StringUtils::to_bold("Usage:") << std::endl
		<< "  " << StringUtils::to_bold(progName) << " [-i <mineruletextfile>] [-m '<mineruletext>'] " << std::endl
		<< "     -f <mineruleoptionfile> -O <optionlist> -v -h " << std::endl << std::endl;
	
	std::cout 
		<< StringUtils::to_bold("   -i") << " -- specify a file name containing the text of the minerule" << std::endl
		<< StringUtils::to_bold("   -m") << " -- the argument is the text of the minerule" << std::endl
		<< StringUtils::to_bold("   -f") << " -- specify a file name containing the Options to be used." << std::endl
		<< "         more than one is allowed and later ones override the options"<< std::endl
		<< "         read from previous ones" << std::endl
		<< StringUtils::to_bold("   -O") << " -- Allows to specify some options from the command line" << std::endl
		<< "         whether those options overrides or not the ones given by -f"<<std::endl
		<< "         commands depends on which comes first" <<std::endl
		<< StringUtils::to_bold("   -v") << " -- version informations" << std::endl
		<< StringUtils::to_bold("   -h") << " -- this message" << std::endl
		<< StringUtils::to_bold(" exit codes:") << std::endl;
	for(int i=me_error_begin(); i<me_error_end(); i++) {
		std::cout 
		<<  "\t" << i << "\t- " << me_error_name((MineruleErrors) i) << std::endl;
	}
  
	exit(0);
}

void printVersion() {
	std::cout << StringUtils::to_bold("Minerule v:") << MR_VERSION << std::endl;
}


/*
* Tries to parse mrtext in order to find the name of
	* the current minerule. 
 */
std::string 
parseMineruleName(const std::string& mrtext) {
	std::string mrtextcopy = mrtext;
	std::string spacechars = " \t\n";
  
	std::string mstringrule="mine rule ";
	std::string mstringsequence="mine sequence";
	std::string mstringitemset="mine itemset";
	std::string mstring;

	size_t minerulepos;
	size_t startnamepos = string::npos;
	size_t endnamepos = string::npos;

  // strlwr seems to be not standard and hence not
  // present in all systems. The following substitue it
  // (it is needed in order to implement case insensitivity)
	for(size_t i=0; i<mrtext.size(); i++) {
		mrtextcopy[i]=tolower(mrtextcopy[i]);
	}

	if(mrtextcopy.find(mstringrule,0)!=string::npos)
		mstring=mstringrule;
	else if(mrtextcopy.find(mstringitemset,0)!=string::npos)
		mstring=mstringitemset;
	else
		mstring=mstringsequence;
  
	minerulepos = mrtextcopy.find(mstring,0);
  
	if(minerulepos!=string::npos) 
		startnamepos = mrtextcopy.find_first_not_of(spacechars ,
			minerulepos+mstring.size());

	if(startnamepos!=string::npos)
		endnamepos = mrtextcopy.find_first_of(spacechars,startnamepos);

  
	if(endnamepos==string::npos ) {
    // notice we will be here, if any of the searches above fails
		MRErr() << "Error in retrieving the minerule name from the " 
			<< "text which define the minerule. I could not "
			<< "initialize, then, the text for %m parameter. " 
			<< "I'm now switching to the default 'mrnameerror' mine rule name" << std::endl
			<< "NOTE: If the minerule main parser succeed in parsing the" << std::endl
			<< " minerule text, then, this message is a BUG and should be"<<std::endl
			<< " reported!" << std::endl;
		return "mrnameerror";
	} 

	return mrtext.substr(startnamepos, endnamepos-startnamepos);
}


void
parseOptions(int argc, char** argv, MineruleOptions& mrOpts, std::string& mrtext) {
	int opt;
	bool okMRText=false;
	bool okMROptions=false;
	std::vector<std::string> cmd_line_mr_options;
	
	while((opt=getopt(argc,argv,"hi:f:m:O:ev"))!=-1) {
		switch(opt) {
			case 'h':
				printVersion();
				printHelp(argv[0]);
				exit(0);
			case 'v':
				printVersion();
				exit(0);
			case 'i':
			{
				if(okMRText) {
					MRErr() << "More than one minerule has been specified! "
						"(you cannot specify more than one {-i,-m} options)" << std::endl;

					std::cerr  << "More than one minerule has been specified! "
						"(you cannot specify more than one {-i,-m} options)" << std::endl;
					exit(MR_ERROR_OPTION_PARSING);
				}
				ifstream infile(optarg);
				if(!infile) {
					MRErr() << "Cannot open input file" << std::endl;
					std::cerr <<  "Cannot open input file" << std::endl;
					exit(MR_ERROR_INPUT_FILE_NOT_FOUND);
				}
				std::string buf;
				mrtext = "";
				while( infile >> buf ) {
					mrtext += " " + buf;
				}

				okMRText=true;
				mrOpts.setMineruleName(parseMineruleName(mrtext));
				mrOpts.setMineruleSourceName(optarg);
			}
			break;
			case 'f':
				if( FileUtils::fileExists(optarg) ) {
					mrOpts.readFromFile(optarg);
					MRLog() << "Options read from '" << optarg <<"'." << std::endl;
				} else {
					MRLog() << optarg << " not found, starting with default Options" << std::endl;
				}
				okMROptions = true;
				break;
			case 'O':
				cmd_line_mr_options.push_back(string(optarg));
				break;
			case 'm':
				if( okMRText ) {
					MRErr() << "More than one minerule has been specified! "
						"(you cannot specify more than one {-i,-m} options)" << std::endl;

					std::cerr  << "More than one minerule has been specified! "
						"(you cannot specify more than one {-i,-m} options)" << std::endl;
					exit(MR_ERROR_OPTION_PARSING);
				}
				mrtext=optarg;
				okMRText=true;
				mrOpts.setMineruleName(parseMineruleName(mrtext));
				mrOpts.setMineruleSourceName("MRFromSTDIN");
				break;
			default:
				MRErr() << "Option not recognized:" << (char)opt << std::endl;
				std::cerr <<  "Option not recognized:" << (char)opt << std::endl;
				exit(MR_ERROR_OPTION_PARSING);
		}
	}

	if(!okMRText) {
		MRErr() << "No minerule has been specified (use -i or -m options)" << std::endl;
		exit(MR_ERROR_NO_MINERULE_SPECIFIED);
	}
	
	if(!okMROptions) {
		if( FileUtils::fileExists(MineruleOptions::DEFAULT_FILE_NAME) ) {
			mrOpts.readFromFile(MineruleOptions::DEFAULT_FILE_NAME);
			MRLog() << "Options read from:" << MineruleOptions::DEFAULT_FILE_NAME << std::endl;
			okMROptions = true;
		} 
		
		if( !cmd_line_mr_options.empty() ) {
			for(std::vector<std::string>::const_iterator it=cmd_line_mr_options.begin(); it!=cmd_line_mr_options.end(); ++it) {
				mrOpts.readFromString(*it);
				MRLog() << "Options set accordingly to command line paramenter:-O " << *it << std::endl;
			}
			okMROptions = true;			
		}
		
		
		if(!okMROptions) {
			MRErr() << "No option file specified or found.";
			exit(MR_ERROR_NO_OPTIONFILE_SPECIFIED);
		}
	}
}

int main (int argc, char *argv[])
{	
	try {
		MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
		std::string mrtext;
		
		parseOptions(argc, argv, mrOpts, mrtext);

		MRLogPush("Initialization...");
		MRLog() << "Minerule source text:" << std::endl;
		MRLog(mrtext);
		MRLogPop();
				
		MRLogPush("Minerule system starting");
		
		OptimizedMinerule optMR(mrtext);
		Algorithms::executeMinerule(optMR);
		
		MRLogPop();
		
		MRLogShowMeasurements();
	} catch (odbc::SQLException& e) {
		MRErr() << "ODBC Exception:" << e.getMessage() << std::endl;
		exit(MR_ERROR_DATABASE_ERROR);
		throw;
	} catch (MineruleException& e) {
		MRErr() << "Minerule Exception:" << e.what() << std::endl;
		exit( e.getErrorCode() );
	} catch (std::exception& e) {
		MRErr() << "Exception:" << e.what() << std::endl;
		exit( MR_ERROR_UNKNOWN );
	}
	catch(...) {
		MRErr() << "An unknown exception has been thrown..." << std::endl;
		exit( MR_ERROR_UNKNOWN );
	}

	return 0;
}


