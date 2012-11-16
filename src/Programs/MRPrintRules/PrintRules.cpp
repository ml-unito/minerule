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

#include "RuleFormatter.h"


using namespace minerule;

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
    std::cout << StringUtils::to_bold("Usage:") << std::endl
    << "   " << StringUtils::to_bold(argv[0]) << " [-0] [-c] [-h] [-s <order>] [-x <sepString>] [-f <optionfile>] resultsetname" << std::endl
	<< "The program allows printing results of minerule queries." << std::endl 
	<< std::endl
	<< StringUtils::to_bold("-0") << " - suppresses logging output " << std::endl
    << StringUtils::to_bold("-c") << " - do not filter out rules having low confidence" << std::endl	
    << StringUtils::to_bold("-s -") << " allows one to sort the rules in a given order." << std::endl
    << "   supported orders are: " << std::endl
    << "     'no' -> no particular order (fastest display)" << std::endl
    << "     'scbh' -> order is support, confidence, body, head"<<std::endl
    << "     'bhsc' -> order is body, head, supp, conf"<<std::endl
    << "     'hbsc' -> order is head, body, supp,  conf"<<std::endl
    << "     'csbh' -> order is conf, supp, body, head" << std::endl
    << "     'cbhs' -> order is conf, body, head, supp" << std::endl
    << "     'cbsh' -> order is conf, body, supp, head" << std::endl
    << "   the default is 'no'" << std::endl
    << StringUtils::to_bold("-x") << " - allows one to specify if a particular std::string needs to be" << std::endl
    << "   used in order to specify fields in the output (if none is given" << std::endl
    << "   the program uses ',')." <<std::endl
    << std::endl << std::endl;
}

void printVersion() {
    std::cout << "PrintRules v:" << MR_VERSION << std::endl;
}

void
parseOptions(int argc, char** argv, MineruleOptions& opt, RuleFormatter*& rf, double& conf) {
    bool didReadMROpts=false;
    std::string sepString="";
	bool suppressLog = false;
		
    rf=NULL;
    for( int i=0; i<argc; i++ ) {
        // checking -f options
        if(argv[i]==string("-f")) {
            if(i+1<argc) {
                if( minerule::FileUtils::fileExists( argv[i+1] ) ) {
                    // all ok
                    opt.readFromFile(argv[i+1]);
                    didReadMROpts=true;
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
        
        if(argv[i]==string("-h")) {
            printHelp(argc, argv);
            exit(0);
        }
        
        if(argv[i]==string("-x")) {
            if((i+1)<argc) {
                sepString = argv[i+1];
            } else {
                std::cerr << "-x option recognized, but its argument is missing!" << std::endl;
                printHelp(argc, argv);
                exit(4);
            }
        }
        
        
        if(argv[i]==string("-v")) {
            printVersion();
            exit(0);
        }
        
        if(argv[i]==string("-c")) {
            conf=0;
        }
		
		if(argv[i]==string("-0")) {
			suppressLog = true;
		}
        
        if(argv[i]==string("-s")) {
            if((i+1)<argc) {
                std::string arg = argv[i+1];
                if(string(arg)=="no") {
                    rf=new SimpleRuleFormatter();
                } else if(string(arg)=="scbh") {
                    rf=new SortedRuleFormatter<QueryResult::SortSuppConfBodyHead>();
                } else if(string(arg)=="bhsc") {
                    rf=new SortedRuleFormatter<QueryResult::SortBodyHeadSuppConf>();
                } else if(string(arg)=="hbsc") {
                    rf=new SortedRuleFormatter<QueryResult::SortHeadBodySuppConf>();
                } else if(string(arg)=="csbh") {
                    rf=new SortedRuleFormatter<QueryResult::SortConfSuppBodyHead>();
                } else if(string(arg)=="cbhs") {
                    rf=new SortedRuleFormatter<QueryResult::SortConfBodyHeadSupp>();
                } else if(string(arg)=="cbsh") {
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
    
    if( !didReadMROpts ) {
        if(FileUtils::fileExists(MineruleOptions::DEFAULT_FILE_NAME))
            opt.readFromFile(MineruleOptions::DEFAULT_FILE_NAME);
        else {
            std::cout << "Cannot open "+MineruleOptions::DEFAULT_FILE_NAME+" and not -f option has been given"
            << std::endl;
            exit(MR_ERROR_NO_OPTIONFILE_SPECIFIED);
        }
    }
    
    if( sepString!="" )
        rf->setFieldSeparationString(sepString);
	
	if( suppressLog )
		rf->setSuppressLog(true);
}


int
main(int argc, char** argv) {
    assert(argc>1);
    // The last parameter MUST be the query name
    std::string qryname = argv[argc-1];
    
    try {
        MineruleOptions& mr = MineruleOptions::getSharedOptions();
        RuleFormatter* rf=NULL;
        double conf=-1;
        parseOptions(argc, argv, mr, rf, conf);
        
        if(!rf->suppressLog()) MRLogPush("Printing result set...");
        printRules( qryname, *rf, conf);
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
