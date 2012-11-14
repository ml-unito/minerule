#include "Algorithms/ConstrTree.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Utils/MineruleOptions.h"
#include "Algorithms/PrepareDataUtils.h"
#include <iterator>




namespace minerule {

  //per algo costruttivo: nella struttura non metto i supporti a zero
  void ConstrTree::insertRulesInStructure(){
    QueryResult::Iterator qit;
    //il primo paramentro e' il nome della tabella dei ris della vecchia query
    OptimizerCatalogue::getMRQueryResultIterator(minerule->getOptimizationInfo().minerule.tab_result, qit, -1, 0.0);
    Head* newhead;
    while( qit.next()) {
      QueryResult::Rule r;
      //crea body e head che sono due vettori ItemSet
      //ovvero due vettori di ItemType
      //ogni ItemType contiene un puntatore ad un SourceRowElement
      qit.getRule(r);
      //insertRuleInStructure(r);
      //body e head sono due puntatori a ItemSet
      newhead=root->insertItemSetB(*r.body,0.0);
      newhead->insertItemSetH(*r.head,0.0);
    }

  }



  void ConstrTree::adjustSuppMIndex(){

    while( mb2->current() != mb2->end() ) {
     std::string g=mb2->getCurrentGID();
      ItemSet* body=new ItemSet();
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
	ItemSet* head=new ItemSet();
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
      //std::string g=mb2->getCurrentGID();
      gidb=curRowb.getGroupBody();
    
      ItemSet* body=new ItemSet();
      //cout<<"in adjustSupp:"<<std::endl;

      while(bodynotend && ItemType(curRowb.getGroupBody())==gidb){
	body->push_back(curRowb.getBody());
	if((bodynotend=rb2->next())) {
	  curRowb.init(rb2, bodyDes);
	}
      }
      
      if(headnotend) {
	HeadBodySourceRow curRowh(rh2, headDes);
	if (gidb!=curRowh.getGroupBody())
	  this->root->findBodiesInTree(body);
	else{
	  ItemSet* head=new ItemSet();
	  while(headnotend && ItemType(curRowh.getGroupBody())==gidb ){
	    head->push_back(curRowh.getHead());
	    if ((headnotend=rh2->next())) {
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
    MRLog() << "Reading previous result and preparing data structures..." << std::endl;
    insertRulesInStructure();

    Connection connection;
    connection.setOutTableName(minerule->getParsedMinerule().tab_result);
    connection.useODBCConnection(
       MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection());
    connection.createResultTables(SourceRowDescriptor(connection.getODBCConnection(), minerule->getParsedMinerule()));

    if (mb2!=NULL  &&  mh2!=NULL) {
      adjustSuppMIndex();
      std::vector<ItemType> body;
      getRoot()->extractRules(body,
			      minerule->getParsedMinerule().sup,
			      minerule->getParsedMinerule().conf,
			      ngroups,
			      &connection);
    }
    else {
      if (rb2!=NULL  &&  rh2!=NULL) {
	MRLog() << "Evaluating constraints and adjusting the support counts in the data structure..." << std::endl;
	adjustSuppRSet();
 std::vector<ItemType> body;

	MRLog() << "Pruning rules and writing results..." << std::endl;
	getRoot()->extractRules(body,
				minerule->getParsedMinerule().sup,
				minerule->getParsedMinerule().conf,
				ngroups,
				&connection);
      } else 
	throw MineruleException (MR_ERROR_INTERNAL,
				 " cannot create rules ");
    }
  }


  size_t ConstrTree::buildAttrStr(const ParsedMinerule::AttrVector& attr,
							size_t startIndex,
							std::string& attrStr, 
							std::vector<int>& des) const {
    ParsedMinerule::AttrVector::const_iterator it = attr.begin();
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
  
 std::string ConstrTree::buildQry( const std::string& groupAttrStr,
						     const std::string& attrStr,
						     const std::string& constraints) const {
    
    return std::string("SELECT "+groupAttrStr+","+attrStr+" "
		  "FROM "+minerule->getParsedMinerule().tab_source+" "+
		  (constraints.size()>0 ?
		  "WHERE "+constraints+" " :
		  "")
		  +"ORDER BY "+groupAttrStr);
  }



  void ConstrTree::execute() 
    throw(MineruleException,odbc::SQLException) {
    assert( minerule->getOptimizationInfo().relationship == OptimizedMinerule::Dominance );
    assert( minerule->getParsedMinerule().mc!=NULL &&
	    minerule->getParsedMinerule().mc->next==NULL);

#ifdef SAVE_TIMINGS_ON_FILE
    #warning Temporary code...
    MineruleOptions& opts = MineruleOptions::getSharedOptions();
    ofstream outfile("/home/minerule/mrweb/timings.txt", ios_base::out | ios_base::app);
    assert(outfile);
    outfile << minerule->getParsedMinerule().mc->l_and->sp->val2 << " ";
// temporary code ends
#endif


    MRLogPush("This is the Context Dependent Constructive Mining Algorithm...");
    MRLogPush("Building source information");

    MRLog() << "Separating the constraints in the HEAD and BODY parts..."<<std::endl;
   std::string bodyConstraints;
   std::string headConstraints;
    HeadBodyPredicatesSeparator::separate(minerule->getParsedMinerule().mc->l_and,
					  bodyConstraints,
					  headConstraints);

    MRLog() << "Building db queries" << std::endl;
    size_t index;
   std::string groupAttr;
   std::string bodyAttr;
   std::string headAttr;
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
		 
   std::string bodyQry =
      buildQry( groupAttr,
		bodyAttr,
		bodyConstraints);

   std::string headQry =
      buildQry( groupAttr,
		headAttr,
		headConstraints);

    MRLog() << "Body query" << bodyQry << std::endl;
    MRLog() << "Head query" << headQry << std::endl;
    MRLog() << "Executing queries" << std::endl;
    odbc::Connection* con =
      MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection();

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
    dbg << std::endl;
    MRDebug() << "Body ids:";
    copy( bodyDes.bodyElems.begin(),
	  bodyDes.bodyElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg<< std::endl;
    MRDebug() << "Body query:" << bodyQry << std::endl;
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
    dbg << std::endl;
    MRDebug() << "Head ids:";
    copy( headDes.headElems.begin(),
	  headDes.headElems.end(),
	  std::ostream_iterator<int>(dbg, ",") );
    dbg<< std::endl;
    MRDebug() << "Head query:" << headQry << std::endl;
    MRDebugPop();
    */
    // Debugging code ends

    stateh2 = con->createStatement();
    rh2 = stateh2->executeQuery(headQry);
    ngroups = PrepareDataUtils::evaluateTotGroups(minerule->getParsedMinerule());

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< " ";
//end
#endif

    MRLogPop();

    MRLogPush("Starting the mining algorithm...");
    adjustSupp();

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta() << " ";
//end    
#endif

    MRLogPop();

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< std::endl;
//end
#endif
    MRLogPop();

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary code
outfile.close();
//end
#endif
    
    // Trashing the trashable
    delete rh2;
    delete stateh2;
    delete rb2;
    delete stateb2;
    rb2=rh2=NULL;
    stateh2=stateb2=NULL;
  }
} // namespace
