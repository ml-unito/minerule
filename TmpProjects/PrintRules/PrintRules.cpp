#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>

#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizerCatalogue.h"
#include "RuleFormatter.h"
#include "MRDatabase/sourcerowelement.h"

using namespace std;
using namespace minerule;


/*
void
readElems( odbc::PreparedStatement* state,
	   int id,
	   vector<string>& elems ) {
  state->setInt(1,id);
  odbc::ResultSet* rs = state->executeQuery();

  while( rs->next() ) {
    string str = rs->getString(1);
    try {
      SourceRowElement* elem = SourceRowElement::deserializeElementFromString(str);
      elems.push_back(elem->asString());
      delete elem;
    } catch (MineruleException& e) {
      elems.push_back(str);
    }
  }

  sort(elems.begin(), elems.end());

  delete rs;
  }*/




void
printRules( string queryname,
	    RuleFormatter& formatter,
	    double conf) throw (MineruleException, odbc::SQLException, std::exception) {
  QueryResult::Iterator qit;
  OptimizerCatalogue::getMRQueryResultIterator( queryname, qit, -1, conf );

  while( qit.next() ) {
    QueryResult::Rule r;
    qit.getRule(r);

    formatter.printRule(r);
  }
  
  formatter.postExec();
}


bool
fileExists(const string& filename) {
  struct stat st;
  if( stat(filename.c_str(),&st)==0 && S_ISREG(st.st_mode) ) {
    return true;
  } else {
    return false;
  }
}



void
printHelp(int argc, char** argv) {
  cout << "Usage:" << endl
       << "   " << argv[0] << " [-c] [-h] [-s <order>] [-x <sepString>] [-f <optionfile>] resultsetname" << endl
       << " -c do not filter out rules having low confidence" << endl
       << " Option -s can be used to sort the rules in a given order." << endl
       << "   supported orders are: " << endl
       << "     'no' -> no particular order (fastest display)" << endl
       << "     'scbh' -> order is support, confidence, body, head"<<endl
       << "     'bhsc' -> order is body, head, supp, conf"<<endl
       << "     'hbsc' -> order is head, body, supp,  conf"<<endl
       << "     'csbh' -> order is conf, supp, body, head" << endl
       << "     'cbhs' -> order is conf, body, head, supp" << endl
       << "     'cbsh' -> order is conf, body, supp, head" << endl
       << "   the default is 'no'" << endl
       << " Option -x allows one to specify if a particular string needs to be" << endl
       << "   used in order to specify fields in the output (in none is given)" << endl
       << "   the program uses a human readable format." <<endl
       << endl << endl;
}

void printVersion() {
  cout << "PrintRules v:" << MR_VERSION << endl;
}

void
parseOptions(int argc, char** argv, MineruleOptions& opt, RuleFormatter*& rf, double& conf) {
  bool didReadMROpts=false;
  string sepString="";
  rf=NULL;
  for( int i=0; i<argc; i++ ) {
    // checking -f options
    if(argv[i]==string("-f")) {
      if(i+1<argc) {
	if( fileExists( argv[i+1] ) ) {
	  // all ok
	  opt.readFromFile(argv[i+1]);
	  didReadMROpts=true;
	} else {
	  string errstr;
	  if(errno==0) {
	    errstr = "Not a regular file!";
	  } else {
	    errstr = strerror(errno);
	  }

	  // file argv[i+1] does not exists
	  cout << "Could not open file:" << argv[i+1] <<endl
	       << "The reason is:" << errstr << endl;
	  exit(1);
	}
      } else {
	// -f given, but argv[i+1] is defined
	cout << "Error parsing arguments!" << endl;
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
	cerr << "-x option recognized, but its argument is missing!" << endl;
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

    if(argv[i]==string("-s")) {
      if((i+1)<argc) {
	string arg = argv[i+1];
	if(string(arg)=="no") {
	  rf=new SimpleRuleFormatter(cout);
	} else if(string(arg)=="scbh") {
	  rf=new SortedRuleFormatter<QueryResult::SortSuppConfBodyHead>(cout);
	} else if(string(arg)=="bhsc") {
	  rf=new SortedRuleFormatter<QueryResult::SortBodyHeadSuppConf>(cout);
	} else if(string(arg)=="hbsc") {
	  rf=new SortedRuleFormatter<QueryResult::SortHeadBodySuppConf>(cout);
	} else if(string(arg)=="csbh") {
	  rf=new SortedRuleFormatter<QueryResult::SortConfSuppBodyHead>(cout);
	} else if(string(arg)=="cbhs") {
	  rf=new SortedRuleFormatter<QueryResult::SortConfBodyHeadSupp>(cout);
	} else if(string(arg)=="cbsh") {
	  rf=new SortedRuleFormatter<QueryResult::SortConfBodySuppHead>(cout);
	}
      } else {
	cerr << "-s option recognized, but its argument is missing!" << endl;
	printHelp(argc,argv);
	exit(3);
      }
    }
  }

  if(rf==NULL) {
    rf = new SimpleRuleFormatter(cout);
  }
  
  if( !didReadMROpts ) {
    if(fileExists("mroptions"))
      opt.readFromFile("mroptions");
    else {
      cout << "Cannot open 'mroptions' and not -f option has been given"
	   << endl;
      exit(1);
    }
  }

  if( sepString!="" )
    rf->setFieldSeparationString(sepString);
}


int
main(int argc, char** argv) {
  assert(argc>1);
  // The last parameter MUST be the query name
  string qryname = argv[argc-1];

  try {
    MineruleOptions& mr = MineruleOptions::getSharedOptions();
    RuleFormatter* rf=NULL;
    double conf=-1;
    parseOptions(argc, argv, mr, rf, conf);

    MRErr() << "Started..." << endl;
    printRules( qryname, *rf, conf);
    MRErr() << "Done!" << endl;
  } catch ( minerule::MineruleException& e ) {
    MRErr() << "MineruleError:" << e.what() << endl;
    throw;
  }  catch( odbc::SQLException& e) {
    MRErr() << "SQLError:" << e.what() << endl;
    throw;
  }  catch (std::exception& e) {
    MRErr() << "Couldn't execute your request, the reason is:" 
	    << e.what() << endl;
    throw;
  }  catch (...) {
    MRErr() << "Uncought exception!" << endl;
    throw;
  }

  return 0;
}
