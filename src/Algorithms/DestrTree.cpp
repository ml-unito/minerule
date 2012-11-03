#include "Algorithms/DestrTree.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include "Utils/MineruleOptions.h"
#include <iterator>
#include "Algorithms/PrepareDataUtils.h"
#include <cmath>


using namespace std;

namespace minerule {

  void DestrTree::insertRulesInStructure(){
    QueryResult::Iterator qit;
    //il primo paramentro e' il nome della tabella dei ris della vecchia query
    OptimizerCatalogue::getMRQueryResultIterator(minerule->getOptimizationInfo().minerule.tab_result, qit, -1, 0.0);
    Head* newhead;
    while( qit.next()) {
      QueryResult::Rule r;
      //crea body e head che sono due vettori ItemSetType
      //ovvero due vettori di ItemType
      //ogni ItemType contiene un puntatore ad un SourceRowElement
      qit.getRule(r);
      //insertRuleInStructure(r);
      //body e head sono due puntatori a ItemSetType
      /*ItemSetType:: iterator itr=(r.body)->begin();
		        while(itr!=(r.body)->end()){cout<<" "<<(*itr);itr++;}
			cout<<" -> ";
      itr=(r.head)->begin();
		        while(itr!=(r.head)->end()){cout<<" "<<(*itr);itr++;}
			cout<<endl;*/
      double suppr=round(r.support*ngroups);
      //cout<<"suppr "<<suppr<<endl;
      double suppb=round(suppr/r.confidence);
      //cout<<"suppb "<<suppb<<endl;
      //cout<<endl;
      newhead=root->insertItemSetB(*r.body,suppb);
      newhead->insertItemSetH(*r.head,suppr);
    }

  }


   void DestrTree::adjustSuppMIndex(){
     //    MRLog() << "findRulesInTree da adjustSuppMIndex()..."<<endl;
    bool notend=1;
    //body e head servono per capire quando devo andare avanti nella rispettiva lista oppure no
    bool newbody=1;
    bool newhead=1;
    ItemSetType* b1=new ItemSetType();
    ItemSetType* b1nb2=new ItemSetType();
    ItemSetType* h1=new ItemSetType();
    ItemSetType* h1nh2=new ItemSetType();
    while( notend ) {
      string gb,gh;
      if (newbody){delete b1; delete b1nb2;
                ItemSetType* b1=new ItemSetType();
                ItemSetType* b1nb2=new ItemSetType();
		gb=mb1->getCurrentGID();
                for(; mb1->current() != mb1->end() && mb1->getCurrentGID()==gb;(*mb1)++){
	         ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb1->getCurrentItem()));
	         b1->push_back(item);
                }
                for(; mb1nb2->current() != mb1nb2->end() && mb1->getCurrentGID()==gb;(*mb1nb2)++){
	         ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb1->getCurrentItem()));
	         b1nb2->push_back(item);
                }
      }
      if (newhead){delete h1; delete h1nh2;
                ItemSetType* h1=new ItemSetType();
                ItemSetType* h1nh2=new ItemSetType();
                gh=mb1->getCurrentGID();
                for(; mh1->current() != mh1->end() && mh1->getCurrentGID()==gh;(*mh1)++){
	         ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh1->getCurrentItem()));
	         h1->push_back(item);
                }
                for(; mh1nh2->current() != mh1nh2->end() && mh1->getCurrentGID()==gh;(*mh1nh2)++){
	         ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh1->getCurrentItem()));
	         h1nh2->push_back(item);
                }
      }

      if (gb>gh) {

		  root->findRulesInTree(NULL, NULL, h1, h1nh2);
                  newbody=0;newhead=1;
      }else if (gb<gh) {
			root->findRulesInTree(b1, b1nb2, NULL, NULL);
                        newbody=1;newhead=0;

       }
       else if (gb==gh) {

			 root->findRulesInTree(b1, b1nb2, h1, h1nh2);
                         newbody=1;newhead=1;

       }

      if (mb1->current()==mb1->end() && mh1->current()==mh1->end()) notend=0;
    }
  }



  void DestrTree::adjustSuppRSet(){
    MRLog() << "findRulesInTree da adjustSuppRSet()..."<<endl;
    ItemType gb;
    ItemType gh;
    bool newbody=1;
    bool newhead=1;
    bool notend=1;
    bool b1notend=rb1->next();
    bool h1notend=rh1->next();
    bool b1nb2notend=rb1nb2->next();
    bool h1nh2notend=rh1nh2->next();
    ItemSetType* b1=new ItemSetType();
    ItemSetType* b1nb2=new ItemSetType();
    //HeadBodySourceRow curRowb1(rb1, bodyDes);
    //HeadBodySourceRow curRowb12(rb1nb2, bodyDes);
    ItemSetType* h1=new ItemSetType();
    ItemSetType* h1nh2=new ItemSetType();
    //HeadBodySourceRow curRowh1(rh1, headDes);
    //HeadBodySourceRow curRowh12(rh1nh2, headDes);
    while( notend ) {
      
      if (b1notend && newbody){
	delete b1; delete b1nb2;
	b1=new ItemSetType();
	b1nb2=new ItemSetType();
        HeadBodySourceRow curRowb1(rb1, bodyDes);
        gb=curRowb1.getGroupBody();

        while(b1notend && ItemType(curRowb1.getGroupBody())==gb){
	  b1->push_back(curRowb1.getBody());
	  if((b1notend=rb1->next())) {
	    curRowb1.init(rb1, bodyDes);
	  }
        }
        if(b1nb2notend){
         HeadBodySourceRow curRowb12(rb1nb2, bodyDes);
         while(b1nb2notend && ItemType(curRowb12.getGroupBody())==gb){
	   b1nb2->push_back(curRowb12.getBody());
	   if((b1nb2notend=rb1nb2->next())) {
	     curRowb12.init(rb1nb2, bodyDes);
	   }
         }
	}
      }
      


      if(h1notend && newhead) {
	delete h1; delete h1nh2;
        h1=new ItemSetType();
	h1nh2=new ItemSetType();
	HeadBodySourceRow curRowh1(rh1, headDes);
        gh=curRowh1.getGroupBody();

	  while(h1notend && ItemType(curRowh1.getGroupBody())==gh ){
	    h1->push_back(curRowh1.getHead());
	    if ((h1notend=rh1->next())) {
	      curRowh1.init(rh1, headDes);
	    }
	  }
	  if(h1nh2notend){
           HeadBodySourceRow curRowh12(rh1nh2, headDes);
	   while(h1nh2notend && ItemType(curRowh12.getGroupBody())==gh ){
	     h1nh2->push_back(curRowh12.getHead());
	     if ((h1nh2notend=rh1nh2->next())) {
	       curRowh12.init(rh1nh2, headDes);
	     }
	   }
	  }
       }


      if (!(gb<gh) && !(gb==gh)) {
                        /*cout<<"h1= "<<endl;
                    	ItemSetType:: iterator it1=h1->begin();
		 	 while(it1!=h1->end()){cout<<" "<<(*it1);it1++;}
			 cout<<endl;


		 	 cout<<"h1nh2= "<<endl;
                 	 ItemSetType:: iterator it2=h1nh2->begin();
		 	 while(it2!=h1nh2->end()){cout<<" "<<(*it2);it2++;}
                         cout<<endl;*/


	                 root->findRulesInTree(NULL, NULL, h1, h1nh2);
                         newbody=0;newhead=1;

      }else if (gb<gh) {/*cout<<"b1= "<<endl;
                        ItemSetType:: iterator it3=b1->begin();
		        while(it3!=b1->end()){cout<<" "<<(*it3);it3++;}
			cout<<endl;


			cout<<"b1nb2= "<<endl;
                        ItemSetType:: iterator it4=b1nb2->begin();
		        while(it4!=b1nb2->end()){cout<<" "<<(*it4);it4++;}
                        cout<<endl;*/



                        root->findRulesInTree(b1, b1nb2, NULL, NULL);
                        newbody=1;newhead=0;
       }
       else if (gb==gh) {/*cout<<"b1= "<<endl;
                        ItemSetType:: iterator it5=b1->begin();
		        while(it5!=b1->end()){cout<<" "<<(*it5);it5++;}
			cout<<endl;


			cout<<"b1nb2= "<<endl;
                        ItemSetType:: iterator it6=b1nb2->begin();
		        while(it6!=b1nb2->end()){cout<<" "<<(*it6);it6++;}
			cout<<endl;


			cout<<"h1= "<<endl;
			ItemSetType:: iterator it7=h1->begin();
		        while(it7!=h1->end()){cout<<" "<<(*it7);it7++;}
			cout<<endl;


			cout<<"h1nh2= "<<endl;
                        ItemSetType:: iterator it8=h1nh2->begin();
		        while(it8!=h1nh2->end()){cout<<" "<<(*it8);it8++;}
                        cout<<endl;

			cout<<"stampa terminata"<<endl;*/
                         root->findRulesInTree(b1, b1nb2, h1, h1nh2);
                         newbody=1;newhead=1;
       }

     /*cout<<"gruppo di b1: "<<gb<<endl;
     cout<<"gruppo di h1: "<<gh<<endl;*/
     //cout<<"gruppo di b1nb2: "<<ItemType(curRowb12.getGroupBody() <<endl;
     //cout<<"gruppo di h1nh2: "<<ItemType(curRowh12.getGroupBody()<<endl;
     if (!b1notend && !h1notend) notend=0;


    }
  }


  void DestrTree::adjustSupp(){
    MRLog() << "Preparing the data structures..." << endl;
    insertRulesInStructure();
    MRLog() << "Done!" << endl;
    sqlCoreConn coreConn;
    coreConn.setOutTableName(minerule->getParsedMinerule().tab_result);
    coreConn.useConnection(
       MineruleOptions::getSharedOptions().getOdbc_db().getConnection());
    coreConn.create_db_rule(0);

    if (mb1!=NULL  &&  mh1!=NULL) {
      adjustSuppMIndex();
      vector<ItemType> body;
      getRoot()->extractRules(body,
			      minerule->getParsedMinerule().sup,
			      minerule->getParsedMinerule().conf,
			      ngroups,
			      &coreConn);
    }
    else {
      if (rb1!=NULL  &&  rh1!=NULL) {
	MRLog() << "Adjusting support..." << endl;
	adjustSuppRSet();
	MRLog() << "Done!" << endl;

	vector<ItemType> body;
	MRLog() << "Extracting rules..." << endl;
	getRoot()->extractRules(body,
				minerule->getParsedMinerule().sup,
				minerule->getParsedMinerule().conf,
				ngroups,
				&coreConn);
	MRLog() << "Done!" << endl;
      } else 
	throw MineruleException (MR_ERROR_INTERNAL,
				 " cannot create rules ");
    }
  }


  size_t DestrTree::buildAttrStr(const ParsedMinerule::ListType& attr,
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
  
  string DestrTree::buildQry( const std::string& groupAttrStr,
			      const std::string& attrStr,
			      const std::string& constraints) const {
    
    return string("SELECT "+groupAttrStr+","+attrStr+" "
		  "FROM "+minerule->getParsedMinerule().tab_source+" "+
		  (constraints.size()>0 ?
		  "WHERE "+constraints+" " :
		  "")
		  +"ORDER BY "+groupAttrStr);
  }

  
  string DestrTree::buildQry1NotQry2( const std::string& groupAttrStr,
				      const std::string& attrStr,
				      const std::string& constraint1,
				      const std::string& constraint2) const {
    
    return string("SELECT "+groupAttrStr+","+attrStr+" "
		  "FROM "+minerule->getParsedMinerule().tab_source+" "
		  "WHERE "+constraint1+" AND NOT("+constraint2+") "
		  "ORDER BY "+groupAttrStr);
  }


  void DestrTree::execute()
    throw(MineruleException,odbc::SQLException) {
    assert( minerule->getOptimizationInfo().relationship == OptimizedMinerule::Dominance );
    assert( minerule->getParsedMinerule().mc!=NULL &&
	    minerule->getParsedMinerule().mc->next==NULL);

/*
#warning Temporary code...
    MineruleOptions& opts = MineruleOptions::getSharedOptions();
    ofstream outfile("/home/minerule/mrweb/timings.txt", ios_base::out | ios_base::app);
    assert(outfile);
    outfile << minerule->getParsedMinerule().mc->l_and->sp->val2 << " ";
// temporary code ends
*/

    MRLogPush("This is the Context Dependent Destructive Mining Algorithm...");
    MRLogPush("Building source information");

    MRLog() << "Separating the constraints in the HEAD and BODY parts..."<<endl;
    string q1BodyConstraints;
    string q1HeadConstraints;
    string q2BodyConstraints;
    string q2HeadConstraints;
    HeadBodyPredicatesSeparator::separate(minerule->getOptimizationInfo().minerule.mc!=NULL?
					  minerule->getOptimizationInfo().minerule.mc->l_and:NULL,
					  q1BodyConstraints,
					  q1HeadConstraints);

    MRLog() << "Separating the constraints in the HEAD and BODY parts (query 2)..." << endl;

    HeadBodyPredicatesSeparator::separate(minerule->getParsedMinerule().mc->l_and,
					  q2BodyConstraints,
					  q2HeadConstraints);

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

    string bodyQry1 =
      buildQry( groupAttr,
		bodyAttr,
		q1BodyConstraints);

    string headQry1 =
      buildQry( groupAttr,
		headAttr,
		q1HeadConstraints);

    string bodyQry1NotQry2 =
      buildQry1NotQry2( groupAttr,
		 bodyAttr,
		 q1BodyConstraints,
		 q2BodyConstraints);

    string headQry1NotQry2 =
      buildQry1NotQry2( groupAttr,
		headAttr,
		q1HeadConstraints,
		q2HeadConstraints);

    MRLog() << "Executing queries" << endl;
    odbc::Connection* con =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    odbc::Statement* stateb1, *stateb1nb2;
    odbc::Statement* stateh1, *stateh1nh2;

    // Debugging code starts
    /*
    MRDebugPush("Body description");
    ostream& dbg = MRDebug();
    dbg << "Group ids:";
    copy( bodyDes.groupBodyElems.begin(),
	  bodyDes.groupBodyElems.end(),
	  ostream_iterator<int>(dbg, ",") );
    dbg << endl;
    MRDebug() << "Body ids:";
    copy( bodyDes.bodyElems.begin(),
	  bodyDes.bodyElems.end(),
	  ostream_iterator<int>(dbg, ",") );
    dbg<< endl;
    MRDebug() << "Body query:" << bodyQry << endl;
    MRDebugPop();
    */
    // Debugging code ends

    stateb1 = con->createStatement();
    rb1 = stateb1->executeQuery(bodyQry1);


    // Debugging code starts
    /*
    MRDebugPush("Head description");
    MRDebug() << "Group ids:";
    copy( headDes.groupBodyElems.begin(),
	  headDes.groupBodyElems.end(),
	  ostream_iterator<int>(dbg, ",") );
    dbg << endl;
    MRDebug() << "Head ids:";
    copy( headDes.headElems.begin(),
	  headDes.headElems.end(),
	  ostream_iterator<int>(dbg, ",") );
    dbg<< endl;
    MRDebug() << "Head query:" << headQry << endl;
    MRDebugPop();
    */
    // Debugging code ends

    stateh1 = con->createStatement();
    rh1 = stateh1->executeQuery(headQry1);


    stateb1nb2 = con->createStatement();
    rb1nb2 = stateb1nb2->executeQuery(bodyQry1NotQry2);

    stateh1nh2 = con->createStatement();
    rh1nh2 = stateh1nh2->executeQuery(headQry1NotQry2);

    ngroups = PrepareDataUtils::evaluateTotGroups(minerule->getParsedMinerule());

    /*
#warning temporary
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< " ";
//end*/
    MRLogPop();

    MRLogPush("Starting the mining algorithm...");
    adjustSupp();
/*#warning temporary
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta() << " ";
//end*/
    MRLogPop();


/*#warning temporary
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< endl;
//end*/
    MRLogPop();

/*#warning temporary code
outfile.close();
//end*/

    // Trashing the trashable
    delete rh1;
    delete stateh1;
    delete rb1;
    delete stateb1;
    rb1=rh1=NULL;
    stateh1=stateb1=NULL;
  }
} // namespace