#include <odbc++/resultset.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include "Database/SourceRowElement.h"
#include <sys/stat.h>
#include <unistd.h>
#include "Database/ItemType.h"
#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizerCatalogue.h"
//#include "Optimizer/QueryResultIterator.h"
#include "classi.h"
#include "algo.h"
#include "Database/SourceRowElement.h"
#include <stdio.h>
#include <string.h>
#include "MRDatabase/MIndex.h"

using namespace std;
using namespace minerule;

//ricorda: da ItemType.h:
//typedef std::vector<ItemType> ItemSetType;
//Ogni ItemType contiene un puntatore a SourceRowElement
//void insertRuleInStructure(const QueryResultIterator::Rule& r){
//    Body* root = new Body();
//    Head* newhead;
//    //body e head sono due puntatori a ItemSetType
//    newhead=root->insertItemSetB(*r.body);
//    newhead->insertItemSetH(*r.head);
//}

/*naviga l'albero BHT per estrarre regole
 - parto dalla root
   visita livello:
 - 1per ogni elem di NRB se supp e' suff:
   2    vado nella head corrispondente:
   3      per ogni elem nel NR:
   4       se supp e' suff:
   5	     estraggo regola
   6	   vado avanti nel NR
   7    per ogni nodo figlio degli elem in NRB: visita livello

suppB e suppR sono i valori soglia del supporto e della regola
*/


void ProvaHead(Head* h){
 Head::RowContainer::iterator it;
 it=h->NR->begin();
 while (it!=h->NR->end()){
 cout<<it->first<<std::endl;
 it++;
 }
}




int
main(int argc, char** argv) {
  assert(argc>2);
  // The last parameter MUST be the query name
  string qryname = argv[argc-1];
  int groups=9983;
  try {
    MRLogPush("inizio");
    MineruleOptions& mr = MineruleOptions::getSharedOptions();
    mr.readFromFile(argv[argc-2]);
    ConstrTree* constr=new ConstrTree;
    Body* r=constr->insertRulesInStructure(qryname,groups);
    //prendere i valori di soglia per frequenza e confidenza e numero gruppi
    vector<ItemType> body;
    vector<int> groupCols;
    groupCols.insert(groupCols.end(), 1);
    vector<int> itemCols;
    itemCols.insert(itemCols.end(), 2);

    MIndex mi1("Sales","descCategory",groupCols,itemCols);
    if(!mi1.existIndex())
      mi1.createIndex();

    MIndex mi2("Sales","descCategory",groupCols,itemCols);
    //if(!mi2.existIndex())
     // mi2.createIndex();

    MIndexIterator * b2;
    MIndexIterator * h2;

    bt_query_t *query = new bt_query_t(bt_query_t::bt_betw,(void*) "a",(void*)"c");

    b2 = new MIndexIterator(mi1, query);
    h2 = new MIndexIterator(mi2, query);


    constr->adjustSupp(b2,h2);

    string tabResult="mrBB4_from_mrArianna5";
    Connection connection;
    //tab_result me lo passera' l'ottimizzatore, per il momento mi invento io un nome
    //connection.setOutTableName(minerule->tab_result);
    connection.setOutTableName(tabResult);
    connection.useODBCConnection(MineruleOptions::getSharedOptions().getOdbc_db().getConnection());
    connection.create_db_rule(0);

    r->extractRules(body,0.012,0.96,constr->getNGroups(),&connection);
    
    delete h2;
    delete b2;
    delete r;
    
    cout<<"c'e ancora qualche body allocato? "<<Body::countb<<std::endl;
    cout<<"c'e ancora qualche head allocata? "<<Head::counth<<std::endl;

    MRErr() << "Done!" << endl;
    MRLogPop();
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
