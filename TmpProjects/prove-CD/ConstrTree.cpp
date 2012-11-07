#include "ConstrTree.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Utils/MineruleOptions.h"
#include <iterator>


using namespace std;

namespace minerule {

  //per algo costruttivo: nella struttura non metto i supporti a zero
  void ConstrTree::insertRulesInStructure(){
    QueryResultIterator qit;
    //il primo paramentro e' il nome della tabella dei ris della vecchia query
    OptimizerCatalogue::getMRQueryResultIterator(minerule->getOptimizationInfo().minerule.tab_result, qit);
    Head* newhead;
    while( qit.next()) {
      QueryResultIterator::Rule r;
      //crea body e head che sono due vettori ItemSetType
      //ovvero due vettori di ItemType
      //ogni ItemType contiene un puntatore ad un SourceRowElement
      qit.getRule(r);
      //insertRuleInStructure(r);
      //body e head sono due puntatori a ItemSetType
      newhead=root->insertItemSetB(*r.body,0.0);
      newhead->insertItemSetH(*r.head,0.0);
    }

  }



  void ConstrTree::adjustSuppMIndex(){

    while( mb2->current() != mb2->end() ) {
      string g=mb2->getCurrentGID();
      ItemSetType* body=new ItemSetType();
      //cout<<"in adjustSupp:"<<std::endl;
      for(; mb2->current() != mb2->end() && mb2->getCurrentGID()==g;(*mb2)++){
	ItemType gid(*SourceRowElement::deserializeElementFromString("n "+mb2->getCurrentGID()));
	ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb2->getCurrentItem()));
	//cout<<item.asString()<<" "<<gid.asString()<<std::endl;
	body->push_back(item);
      }
    

      if (g!=mh2->getCurrentGID())
	this->root->findBodiesInTree(body);
      else{
	ItemSetType* head=new ItemSetType();
	for(;mh2->current() != mh2->end() && mh2->getCurrentGID()==g; (*mh2)++){
	  ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh2->getCurrentItem()));
	  head->push_back(item);
	}
	this->root->findRulesInTree(body,head);
	delete head;
      }
      delete body;
    }
  }

  void ConstrTree::adjustSuppRSet(){
    ItemType gid;
    ItemType gidb;
    ItemType item;
    
    bool bodynotend=rb2->next();
    bool headnotend=rh2->next();
    while( bodynotend ) {

      HeadBodySourceRow curRowb(rb2, bodyDes);
      //string g=mb2->getCurrentGID();
      gidb=curRowb.getGroupBody();
    
      ItemSetType* body=new ItemSetType();
      //cout<<"in adjustSupp:"<<std::endl;

      while(bodynotend && ItemType(curRowb.getGroupBody())==gidb){
	body->push_back(curRowb.getBody());
	if(bodynotend=rb2->next()) {
	  curRowb.init(rb2, bodyDes);
	}
      }
      
      if(headnotend) {
	HeadBodySourceRow curRowh(rh2, headDes);
	if (gidb!=curRowh.getGroupBody())
	  this->root->findBodiesInTree(body);
	else{
	  ItemSetType* head=new ItemSetType();
	  while(headnotend && ItemType(curRowh.getGroupBody())==gidb ){
	    head->push_back(curRowh.getHead());
	    if (headnotend=rh2->next()) {
	      curRowh.init(rh2, headDes);
	    }
	  }

	  this->root->findRulesInTree(body,head);
	  delete head;
	}
      }
      delete body;
    }
  }


  void ConstrTree::adjustSupp(){

    insertRulesInStructure();
    sqlCoreConn coreConn;
    coreConn.setOutTableName(minerule->getParsedMinerule().tab_result);
    coreConn.useConnection(
       MineruleOptions::getSharedOptions().getOdbc_db().getConnection());
    coreConn.create_db_rule(0);

    if (mb2!=NULL  &&  mh2!=NULL) {
      adjustSuppMIndex();
      vector<ItemType> body;
      getRoot()->extractRules(body,
			      minerule->getParsedMinerule().sup,
			      minerule->getParsedMinerule().conf,
			      ngroups,
			      &coreConn);
    }
    else {
      if (rb2!=NULL  &&  rh2!=NULL) {
	adjustSuppRSet();
	vector<ItemType> body;
	getRoot()->extractRules(body,
				minerule->getParsedMinerule().sup,
				minerule->getParsedMinerule().conf,
				ngroups,
				&coreConn);
      } else 
	throw MineruleException (MR_ERROR_INTERNAL,
				 " cannot create rules ");
    }
  }


  size_t ConstrTree::buildAttrStr(const ParsedMinerule::ListType& attr,
							size_t startIndex,
							std::string& attrStr, 
							std::vector<int>& des) const {
    ParsedMinerule::ListType::const_iterator it = attr.begin();
    for( ; it!=attr.end(); it++ ) {
      if(it!=attr.begin()) {
	attrStr+=",";
      }

      attrStr+=*it;
      des.push_back(++startIndex);
    }

    return startIndex;
  }


  // Preprocessing functions
  
  string ConstrTree::buildQry( const std::string& groupAttrStr,
						     const std::string& attrStr,
						     const std::string& constraints) const {
    
    return string("SELECT "+groupAttrStr+","+attrStr+" "
		  "FROM "+minerule->getParsedMinerule().tab_source+" "
		  "WHERE "+constraints+" "
		  "ORDER BY "+groupAttrStr);
  }



  void ConstrTree::execute() 
    throw(MineruleException,odbc::SQLException) {
    assert( minerule->getOptimizationInfo().relationship == OptimizedMinerule::Dominance );
    assert( minerule->getParsedMinerule().mc!=NULL &&
	    minerule->getParsedMinerule().mc->next==NULL);


#warning Temporary code...
    MineruleOptions& opts = MineruleOptions::getSharedOptions();
    ofstream outfile("/home/minerule/mrweb/timings.txt", ios_base::out | ios_base::app);
    assert(outfile);
    outfile << minerule->getParsedMinerule().mc->l_and->sp->val2 << " ";
// temporary code ends


    MRLogPush("This is the Context Dependent Constructive Mining Algorithm...");
    MRLogPush("Building source information");

    MRLog() << "Separating the constraints in the HEAD and BODY parts..."<<std::endl;
    string bodyConstraints;
    string headConstraints;
    HeadBodyPredicatesSeparator::separate(minerule->getParsedMinerule().mc->l_and,
					  bodyConstraints,
					  headConstraints);

    MRLog() << "Building db queries" << endl;
    size_t index;
    string groupAttr;
    string bodyAttr;
    string headAttr;
    index=buildAttrStr(minerule->getParsedMinerule().ga,
		       0,
		       groupAttr,
		       bodyDes.groupBodyElems );

    headDes.groupBodyElems=bodyDes.groupBodyElems;
    
    buildAttrStr(minerule->getParsedMinerule().ba,
		 index,
		 bodyAttr,
		 bodyDes.bodyElems);
    buildAttrStr(minerule->getParsedMinerule().ha,
		 index,
		 headAttr,
		 headDes.headElems);
		 
    string bodyQry =
      buildQry( groupAttr,
		bodyAttr,
		bodyConstraints);

    string headQry =
      buildQry( groupAttr,
		headAttr,
		headConstraints);

    MRLog() << "Executing queries" << endl;
    odbc::Connection* con =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    odbc::Statement* stateb2;
    odbc::Statement* stateh2;

    // Debugging code starts 
    /*
    MRDebugPush("Body description");
    std::ostream& dbg = MRDebug();
    dbg << "Group ids:";
    copy( bodyDes.groupBodyElems.begin(),
	  bodyDes.groupBodyElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg << endl;
    MRDebug() << "Body ids:";
    copy( bodyDes.bodyElems.begin(),
	  bodyDes.bodyElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg<< endl;
    MRDebug() << "Body query:" << bodyQry << endl;
    MRDebugPop();
    */
    // Debugging code ends 

    stateb2 = con->createStatement();
    rb2 = stateb2->executeQuery(bodyQry);


    // Debugging code starts 
    /*
    MRDebugPush("Head description");
    MRDebug() << "Group ids:";
    copy( headDes.groupBodyElems.begin(),
	  headDes.groupBodyElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg << endl;
    MRDebug() << "Head ids:";
    copy( headDes.headElems.begin(),
	  headDes.headElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg<< endl;
    MRDebug() << "Head query:" << headQry << endl;
    MRDebugPop();
    */
    // Debugging code ends

    stateh2 = con->createStatement();
    rh2 = stateh2->executeQuery(headQry);
    ngroups = minerule->evaluateTotGroups();

#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< " ";
//end
    MRLogPop();

    MRLogPush("Starting the mining algorithm...");
    adjustSupp();
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta() << " ";
//end    
    MRLogPop();


#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< endl;
//end
    MRLogPop();

#warning temporary code
outfile.close();
//end
    
    // Trashing the trashable
    delete rh2;
    delete stateh2;
    delete rb2;
    delete stateb2;
    rb2=rh2=NULL;
    stateh2=stateb2=NULL;
  }
} // namespace
