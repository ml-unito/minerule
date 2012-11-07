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
#include "TmpProjects/PrintRules/RuleFormatter.h"
#include "MRDatabase/sourcerowelement.h"

/* stampa a video le regole con relativo supporto e confidenza*/
/* basta dare in input la tabella risultato IMP:qui non ci sono controlli sull'esistenza della tabella*/
/* per questo vedi PrintRules*/

using namespace std;
using namespace minerule;



string quote(const string& elem) {
  return "["+elem+"]";
}

string quoteElems(const vector<std::string>& elems) {
  string result;

  vector<std::string>::const_iterator it=elems.begin();

  if(it!=elems.end()) {
    result = quote(*it);
    it++;
  }

  for(; it!=elems.end(); it++) {
    result +=  "," + quote(*it);
  }

  return result;
}


void printRule(const vector<std::string>& body,
		const vector<std::string>& head,
		double supp,
		double conf) {
  cout << quoteElems(body) << " => " << quoteElems(head)
     << "\t\t" << supp << "\t" << conf << endl;
}





void
readElems( odbc::PreparedStatement* state,
	   int id,
	   vector<std::string>& elems ) {
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
	    const string tablename) {
  odbc::Statement* state=conn->createStatement();
  string query = "SELECT * FROM "+tablename;

  odbc::PreparedStatement* prepStat =
    conn->prepareStatement(
			   "SELECT elem FROM "+tablename+"_elems "+
			   "WHERE id=?");

  odbc::ResultSet* rs = state->executeQuery(query);

  while( rs->next() ) {
    vector<std::string> body;
    vector<std::string> head;
    double supp;
    double conf;
    
    //crea body come un vettore di stringhe
    //rs->getInt(1)restituisce l'identificatore dell'elemento nella prima colonna  
    readElems(prepStat,rs->getInt(1), body);

    //crea head come un vettore di stringhe
    readElems(prepStat,rs->getInt(2), head);

    supp = rs->getFloat(3);
    conf = rs->getFloat(4);

    printRule(body,head,supp,conf);

    //formatter.printRule(body,head,supp,conf);
  }

  //formatter.postExec();

  delete rs;
  delete prepStat;
  delete state;
}





int
main(int argc, char** argv) {
  
  //assert(argc>1);

  string tablename = argv[argc-1];
  odbc::Connection* connection;
  try {
    //MineruleOptions& mr = MineruleOptions::getSharedOptions();

    //parseOptions(argc, argv, mr, rf);
    connection=
	odbc::DriverManager::getConnection("test", "root", "mysql");

    string resultsetname;

    try {
      //resultsetname = OptimizerCatalogue::getResultsetName(tablename);
    } catch (MineruleException& mr) {
      MRWarn() << endl
	       << "The optimizer catalogue does not report any minerule" <<std::endl
	       << "having the name you specified. I'm trying to use the" <<std::endl
	       << "name you provided to locate the result set by myself."<<std::endl;
      resultsetname = tablename;
    }
    resultsetname = tablename;
    MRErr() << "Started..." << endl;
    printRules(connection, resultsetname);
    MRErr() << "Done!" << endl;
  } catch (std::exception& e) {
    MRErr() << "Couldn't execute your request, the reason is:" 
	    << e.what() << endl;
    throw;
  }

  return 0;
}
