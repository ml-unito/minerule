//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Arianna Gallo (gallo.arianna@gmail.com)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "minerule/Algorithms/ConstrTree.hpp"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include <iterator>




namespace minerule {

  //per algo costruttivo: nella struttura non metto i supporti a zero
  void ConstrTree::insertRulesInStructure(){
    QueryResult::Iterator qit;
    //il primo paramentro e' il nome della tabella dei ris della vecchia query
    OptimizerCatalogue::getMRQueryResultIterator(minerule->getOptimizationInfo().minerule.tab_result, qit, -1, 0.0);
    Head* newhead;
    while( qit.next()) {
      Rule r;
      //crea body e head che sono due vettori ItemSet
      //ovvero due vettori di ItemType
      //ogni ItemType contiene un puntatore ad un SourceRowElement
      qit.getRule(r);
      //insertRuleInStructure(r);
      //body e head sono due puntatori a ItemSet
      newhead=root->insertItemSetB(r.getBody(),0.0);
      newhead->insertItemSetH(r.getHead(),0.0);
    }

  }



	//   void ConstrTree::adjustSuppMIndex(){
	// 
	//     while( mb2->current() != mb2->end() ) {
	//      std::string g=mb2->getCurrentGID();
	//       ItemSet* body=new ItemSet();
	//       //cout<<"in adjustSupp:"<<std::endl;
	//       for(; mb2->current() != mb2->end() && mb2->getCurrentGID()==g;(*mb2)++){
	// ItemType gid(*SourceRowElement::deserializeElementFromString("n "+mb2->getCurrentGID()));
	// ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb2->getCurrentItem()));
	// //cout<<item.asString()<<" "<<gid.asString()<<std::endl;
	// body->push_back(item);
	//       }
	//     
	// 
	//       if (g!=mh2->getCurrentGID())
	// this->root->findBodiesInTree(body);
	//       else{
	// ItemSet* head=new ItemSet();
	// for(;mh2->current() != mh2->end() && mh2->getCurrentGID()==g; (*mh2)++){
	//   ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh2->getCurrentItem()));
	//   head->push_back(item);
	// }
	// this->root->findRulesInTree(body,head);
	// delete head;
	//       }
	//       delete body;
	//     }
	//   }

  void ConstrTree::adjustSuppRSet(){
    ItemType gid;
    ItemType gidb;
    ItemType item;
    
    bool bodynotend=rb2->next();
    bool headnotend=rh2->next();
    while( bodynotend ) {

      SourceRow curRowb(rb2, bodyDes);
      //std::string g=mb2->getCurrentGID();
      gidb=curRowb.getGroup();
    
      ItemSet* body=new ItemSet();
      //cout<<"in adjustSupp:"<<std::endl;

      while(bodynotend && ItemType(curRowb.getGroup())==gidb){
	body->push_back(curRowb.getBody());
	if((bodynotend=rb2->next())) {
	  curRowb.init(rb2, bodyDes);
	}
      }
      
      if(headnotend) {
	SourceRow curRowh(rh2, headDes);
	if (gidb!=curRowh.getGroup())
	  this->root->findBodiesInTree(body);
	else{
	  ItemSet* head=new ItemSet();
	  while(headnotend && ItemType(curRowh.getGroup())==gidb ){
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
    MRLogPusher _("Starting the mining algorithm...");
	  
    MRLog() << "Reading previous result and preparing data structures..." << std::endl;
    insertRulesInStructure();

    Connection connection;
    connection.setOutTableName(minerule->getParsedMinerule().tab_result);
    connection.useMRDBConnection(
       MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection());
    connection.createResultTables(SourceRowMetaInfo(connection.getMRDBConnection(), minerule->getParsedMinerule()));

    // if (mb2!=NULL  &&  mh2!=NULL) {
    //    adjustSuppMIndex();
    //    std::vector<ItemType> body;
    //    getRoot()->extractRules(body,
    // 			      minerule->getParsedMinerule().sup,
    // 			      minerule->getParsedMinerule().conf,
    // 			      ngroups,
    // 			      &connection);
    //  }
    //  else {
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
    // }
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


  void ConstrTree::prepareData() {
      MRLogPusher _("Building source information");

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
	  index=buildAttrStr(minerule->getParsedMinerule().ga, 0, groupAttr, bodyDes.groupElems );

	  headDes.groupElems=bodyDes.groupElems;
    
	  buildAttrStr(minerule->getParsedMinerule().ba, index, bodyAttr, bodyDes.bodyElems);
	  buildAttrStr(minerule->getParsedMinerule().ha, index, headAttr, headDes.headElems);
		 
	  std::string bodyQry = buildQry( groupAttr, bodyAttr, bodyConstraints);

	  std::string headQry = buildQry( groupAttr, headAttr, headConstraints);

	  MRLog() << "Body query" << bodyQry << std::endl;
	  MRLog() << "Head query" << headQry << std::endl;
	  MRLog() << "Executing queries" << std::endl;
	  mrdb::Connection* con = MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection();

	  stateb2 = con->createStatement();
	  rb2 = stateb2->executeQuery(bodyQry);


	  stateh2 = con->createStatement();
	  rh2 = stateh2->executeQuery(headQry);
	  ngroups = PrepareDataUtils::evaluateTotGroups(minerule->getParsedMinerule());
  }

  void ConstrTree::execute() 
    throw(MineruleException,mrdb::SQLException) {
    assert( minerule->getOptimizationInfo().relationship == OptimizedMinerule::Dominance );
    assert( minerule->getParsedMinerule().mc!=NULL &&
	    minerule->getParsedMinerule().mc->next==NULL);

    MRLogPusher _("This is the Context Dependent Constructive Mining Algorithm...");
	
	prepareData();
    adjustSupp();
  }
} // namespace
