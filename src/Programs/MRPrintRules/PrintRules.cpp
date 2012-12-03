#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>

#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizerCatalogue.h"
#include "Database/SourceRowElement.h"
#include "Utils/FileUtils.h"
#include "Result/RuleFormatter.h"


using namespace minerule;

struct CharPtrComp
{
	const char* _str;
	
	CharPtrComp(const char* str) {
		_str = str;
	}
};

bool operator==(const char* s1, const CharPtrComp& cmp) {
	return !strcmp(s1, cmp._str);
}


void
printRules(std::string queryname,
           RuleFormatter& formatter,
           double conf) throw (MineruleException, odbc::SQLException, std::exception) {
    QueryResult::Iterator qit;
    OptimizerCatalogue::getMRQueryResultIterator( queryname, qit, -1, conf );
    
    while( qit.next() ) {
        Rule r;
        qit.getRule(r);
        
        formatter.printRule(r);
    }
    
    formatter.postExec();
}


void
printHelp(int argc, char** argv) {
    std::cout << StringUtils::toBold("Usage:") << std::endl
    << "   " << StringUtils::toBold(argv[0]) << " [-h] [-0] [-c] [-n minerule-number] [-s <order>] [-f <optionfile>] resultsetname" << std::endl
	<< "The program allows printing results of minerule queries." << std::endl 
	<< std::endl
	<< StringUtils::toBold("-h") << " - prints this message " << std::endl
	<< StringUtils::toBold("-c") << " - suppress colors " << std::endl
	<< StringUtils::toBold("-0") << " - suppresses logging artifacts " << std::endl
    << StringUtils::toBold("-l") << " - do not filter out rules having low confidence" << std::endl	
	<< StringUtils::toBold("-n") << " - specifies a query number to be printed (this is an alternative to" << std::endl
	<< "     specifying the query name)" << std::endl
    << StringUtils::toBold("-s") << " - sorts the rules in a specified order." << std::endl
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

void printVersion() {
    std::cout << "PrintRules v:" << MR_VERSION << std::endl;
}

std::string	getQueryName(size_t queryNumber) {
	return OptimizerCatalogue::getMRQueryName(queryNumber);
}

bool hasOption(int argc, char** argv, const char* opt) {
	return std::find(&argv[0], &argv[argc], CharPtrComp(opt)) != &argv[argc];
}

std::string
parseOptions(int argc, char** argv, MineruleOptions& opt, RuleFormatter*& rf, double& conf) {
    std::string sepString="";
	bool suppressLog = false;
	std::string queryName;
	
    if( !hasOption(argc, argv, "-f") ) {
        if(FileUtils::fileExists(MineruleOptions::DEFAULT_FILE_NAME))
            opt.readFromFile(MineruleOptions::DEFAULT_FILE_NAME);
        else {
            std::cout << "Cannot open "+MineruleOptions::DEFAULT_FILE_NAME+" and not -f option has been given" << std::endl;
            exit(MR_ERROR_NO_OPTIONFILE_SPECIFIED);
        }
    }
	
		
    rf=NULL;
    for( int i=0; i<argc; i++ ) {
        // checking -f options
        if(argv[i]==std::string("-f")) {
            if(i+1<argc) {
                if( minerule::FileUtils::fileExists( argv[i+1] ) ) {
                    // all ok
                    opt.readFromFile(argv[i+1]);
                } else {
                    std::string errstr;
                    if(errno==0) {
                        errstr = "Not a regular file!";
                    } else {
                        errstr = strerror(errno);
                    }
                    
                    // file argv[i+1] does not exists
                    std::cout << "Could not open file:" << argv[i+1] <<std::endl
                    << "The reason is:" << errstr << std::endl;
                    exit(1);
                }
            } else {
                // -f given, but argv[i+1] is defined
                std::cout << "Error parsing arguments!" << std::endl;
                printHelp(argc, argv);
                exit(2);
            }
        }
		
		if(argv[i]==std::string("-c")) {
			StringUtils::setColorsEnabled(false);
		}
		
		if(argv[i]==std::string("-n")) {
			if(i+1<argc && Converter(argv[i+1]).isNumber() ) {
				queryName = getQueryName( Converter(argv[i+1]).toLong() );
			} else {
				std::cerr << "-n option recognized but its argument has not been given or is not a number"
					<< std::endl;
				printHelp(argc,argv);
				exit(5);
			}
		}
        
        if(argv[i]==std::string("-h")) {
            printHelp(argc, argv);
            exit(0);
        }        
        
        if(argv[i]==std::string("-v")) {
            printVersion();
            exit(0);
        }
        
        if(argv[i]==std::string("-l")) {
            conf=0;
        }
		
		if(argv[i]==std::string("-0")) {
			suppressLog = true;
		}
        
        if(argv[i]==std::string("-s")) {
            if((i+1)<argc) {
                std::string arg = argv[i+1];
                if(std::string(arg)=="no") {
                    rf=new SimpleRuleFormatter();
                } else if(std::string(arg)=="scbh") {
                    rf=new SortedRuleFormatter<QueryResult::SortSuppConfBodyHead>();
                } else if(std::string(arg)=="bhsc") {
                    rf=new SortedRuleFormatter<QueryResult::SortBodyHeadSuppConf>();
                } else if(std::string(arg)=="hbsc") {
                    rf=new SortedRuleFormatter<QueryResult::SortHeadBodySuppConf>();
                } else if(std::string(arg)=="csbh") {
                    rf=new SortedRuleFormatter<QueryResult::SortConfSuppBodyHead>();
                } else if(std::string(arg)=="cbhs") {
                    rf=new SortedRuleFormatter<QueryResult::SortConfBodyHeadSupp>();
                } else if(std::string(arg)=="cbsh") {
                    rf=new SortedRuleFormatter<QueryResult::SortConfBodySuppHead>();
                }
            } else {
                std::cerr << "-s option recognized, but its argument is missing!" << std::endl;
                printHelp(argc,argv);
                exit(3);
            }
        }
    }
    
    if(rf==NULL) {
        rf = new SimpleRuleFormatter();
    }
        
	if( suppressLog )
		rf->setSuppressLog(true);
	
	if( queryName.empty() ) {
		queryName = argv[argc-1];
	}
	
	return queryName;
}


int
main(int argc, char** argv) {    
    try {
		if(argc < 2) {
			printHelp(argc, argv);
			exit(0);
		}
		
        MineruleOptions& mr = MineruleOptions::getSharedOptions();
        RuleFormatter* rf=NULL;
        double conf=-1;
        std::string queryName = parseOptions(argc, argv, mr, rf, conf);
        
        if(!rf->suppressLog()) MRLogPush("Printing result set...");
        printRules( queryName, *rf, conf);
		if(!rf->suppressLog()) MRLogPop();
    } catch ( minerule::MineruleException& e ) {
        MRErr() << "MineruleError:" << e.what() << std::endl;
        throw;
    }  catch( odbc::SQLException& e) {
        MRErr() << "SQLError:" << e.what() << std::endl;
        throw;
    }  catch (std::exception& e) {
        MRErr() << "Couldn't execute your request, the reason is:" 
	    << e.what() << std::endl;
        throw;
    }  catch (...) {
        MRErr() << "Uncought exception!" << std::endl;
        throw;
    }
    
    return 0;
}
