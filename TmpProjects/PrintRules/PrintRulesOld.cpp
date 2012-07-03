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
}




void
printRules( odbc::Connection* conn,
	    string tablename,
	    RuleFormatter& formatter) {
  odbc::Statement* state=conn->createStatement();
  string query = "SELECT * FROM " + tablename;

  odbc::PreparedStatement* prepStat = 
    conn->prepareStatement(
			   "SELECT elem FROM "+tablename+"_elems "+
			   "WHERE id=?");

  odbc::ResultSet* rs = state->executeQuery(query);

  while( rs->next() ) {
    vector<string> body;
    vector<string> head;
    double supp;
    double conf;

    readElems(prepStat,rs->getInt(1), body);
    readElems(prepStat,rs->getInt(2), head);
    supp = rs->getFloat(3);
    conf = rs->getFloat(4);
    
    formatter.printRule(body,head,supp,conf);
  }
  
  formatter.postExec();

  delete rs;
  delete prepStat;
  delete state;
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
       << "   " << argv[0] << " [-h] [-s <order>] [-f <optionfile>] resultsetname" << endl
       << " Option -s can be used to sort the rules in a given order." << endl
       << "   supported orders are: " << endl
       << "     'no' -> no particular order (fastest display)" << endl
       << "     'suppconf' -> order is support, then confidence"<<endl
       << "     'body' -> order is body, then head, then supp, then conf"<<endl
       << "     'head' -> order is head, then body, then supp, then conf"<<endl
       << "   the default is 'no'" << endl
       << endl << endl;
}

void
parseOptions(int argc, char** argv, MineruleOptions& opt, RuleFormatter*& rf) {
  bool didReadMROpts=false;
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

    if(argv[i]==string("-s")) {
      if(i<argc) {
	string arg = argv[i+1];
	if(string(arg)=="no") {
	  rf=new SimpleRuleFormatter(cout);
	} else if(string(arg)=="suppconf") {
	  rf=new SortedRuleFormatter<SortSuppConf>(cout);
	} else if(string(arg)=="body") {
	  rf=new SortedRuleFormatter<SortBodyHeadSuppConf>(cout);
	} else if(string(arg)=="head") {
	  rf=new SortedRuleFormatter<SortHeadBodySuppConf>(cout);
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
}


int
main(int argc, char** argv) {
  assert(argc>1);
  // The last parameter MUST be the result set name
  string tablename = argv[argc-1];

  try {
    MineruleOptions& mr = MineruleOptions::getSharedOptions();
    RuleFormatter* rf=NULL;
    parseOptions(argc, argv, mr, rf);

    odbc::Connection* conn = 
      mr.getOdbc_db().getConnection();

    string resultsetname;

    try {
      resultsetname = OptimizerCatalogue::getResultsetName(tablename);
    } catch (MineruleException& mr) {
      MRWarn() << endl
	       << "The optimizer catalogue does not report any minerule" <<endl
	       << "having the name you specified. I'm trying to use the" <<endl
	       << "name you provided to locate the result set by myself."<<endl;
      resultsetname = tablename;
    }
    
    MRErr() << "Started..." << endl;
    printRules(conn, resultsetname, *rf);
    MRErr() << "Done!" << endl;
  } catch (std::exception& e) {
    MRErr() << "Couldn't execute your request, the reason is:" 
	    << e.what() << endl;
    throw;
  }

  return 0;
}
