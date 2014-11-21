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
#include "minerule/Algorithms/DestrTree.hpp"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include <iterator>
#include "minerule/Database/PrepareDataUtils.hpp"
#include <cmath>




namespace minerule {

	void DestrTree::insertRulesInStructure(){
		QueryResult::Iterator qit;

		OptimizerCatalogue::getMRQueryResultIterator(minerule->getOptimizationInfo().minerule.tab_result, qit, -1, 0.0);
		Head* newhead;
		while( qit.next()) {
			Rule r;
			qit.getRule(r);
			double suppr=round(r.getSupport()*ngroups);
			double suppb=round(suppr/r.getConfidence());
			newhead=root->insertItemSetB(r.getBody(),suppb);
			newhead->insertItemSetH(r.getHead(),suppr);
		}

	}


	// void DestrTree::adjustSuppMIndex(){
	//      //    MRLog() << "findRulesInTree da adjustSuppMIndex()..."<<std::endl;
	// 	bool notend=1;
	//     //body e head servono per capire quando devo andare avanti nella rispettiva lista oppure no
	// 	bool newbody=1;
	// 	bool newhead=1;
	// 	ItemSet* b1=new ItemSet();
	// 	ItemSet* b1nb2=new ItemSet();
	// 	ItemSet* h1=new ItemSet();
	// 	ItemSet* h1nh2=new ItemSet();
	// 	while( notend ) {
	// 		std::string gb,gh;
	// 		if (newbody){delete b1; delete b1nb2;
	// 			ItemSet* b1=new ItemSet();
	// 			ItemSet* b1nb2=new ItemSet();
	// 			gb=mb1->getCurrentGID();
	// 			for(; mb1->current() != mb1->end() && mb1->getCurrentGID()==gb;(*mb1)++){
	// 				ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb1->getCurrentItem()));
	// 				b1->push_back(item);
	// 			}
	// 			for(; mb1nb2->current() != mb1nb2->end() && mb1->getCurrentGID()==gb;(*mb1nb2)++){
	// 				ItemType item(*SourceRowElement::deserializeElementFromString("n "+mb1->getCurrentItem()));
	// 				b1nb2->push_back(item);
	// 			}
	// 		}
	// 		if (newhead){delete h1; delete h1nh2;
	// 			ItemSet* h1=new ItemSet();
	// 			ItemSet* h1nh2=new ItemSet();
	// 			gh=mb1->getCurrentGID();
	// 			for(; mh1->current() != mh1->end() && mh1->getCurrentGID()==gh;(*mh1)++){
	// 				ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh1->getCurrentItem()));
	// 				h1->push_back(item);
	// 			}
	// 			for(; mh1nh2->current() != mh1nh2->end() && mh1->getCurrentGID()==gh;(*mh1nh2)++){
	// 				ItemType item(*SourceRowElement::deserializeElementFromString("n "+mh1->getCurrentItem()));
	// 				h1nh2->push_back(item);
	// 			}
	// 		}
	// 
	// 		if (gb>gh) {
	// 
	// 			root->findRulesInTree(NULL, NULL, h1, h1nh2);
	// 			newbody=0;newhead=1;
	// 		}else if (gb<gh) {
	// 			root->findRulesInTree(b1, b1nb2, NULL, NULL);
	// 			newbody=1;newhead=0;
	// 
	// 		}
	// 		else if (gb==gh) {
	// 
	// 			root->findRulesInTree(b1, b1nb2, h1, h1nh2);
	// 			newbody=1;newhead=1;
	// 
	// 		}
	// 
	// 		if (mb1->current()==mb1->end() && mh1->current()==mh1->end()) notend=0;
	// 	}
	// }



	void DestrTree::adjustSuppRSet(){
		MRLog() << "findRulesInTree da adjustSuppRSet()..."<<std::endl;
		ItemType gb;
		ItemType gh;
		bool newbody=1;
		bool newhead=1;
		bool notend=1;
		bool b1notend=rb1->next();
		bool h1notend=rh1->next();
		bool b1nb2notend=rb1nb2->next();
		bool h1nh2notend=rh1nh2->next();
		ItemSet* b1=new ItemSet();
		ItemSet* b1nb2=new ItemSet();
		ItemSet* h1=new ItemSet();
		ItemSet* h1nh2=new ItemSet();

		while( notend ) {      
			if (b1notend && newbody){
				delete b1; delete b1nb2;
				b1=new ItemSet();
				b1nb2=new ItemSet();
				SourceRow curRowb1(rb1, bodyDes);
				gb=curRowb1.getGroup();

				while(b1notend && ItemType(curRowb1.getGroup())==gb){
					b1->push_back(curRowb1.getBody());
					if((b1notend=rb1->next())) {
						curRowb1.init(rb1, bodyDes);
					}
				}
				if(b1nb2notend){
					SourceRow curRowb12(rb1nb2, bodyDes);
					while(b1nb2notend && ItemType(curRowb12.getGroup())==gb){
						b1nb2->push_back(curRowb12.getBody());
						if((b1nb2notend=rb1nb2->next())) {
							curRowb12.init(rb1nb2, bodyDes);
						}
					}
				}
			}
      


			if(h1notend && newhead) {
				delete h1; delete h1nh2;
				h1=new ItemSet();
				h1nh2=new ItemSet();
				SourceRow curRowh1(rh1, headDes);
				gh=curRowh1.getGroup();

				while(h1notend && ItemType(curRowh1.getGroup())==gh ){
					h1->push_back(curRowh1.getHead());
					if ((h1notend=rh1->next())) {
						curRowh1.init(rh1, headDes);
					}
				}
				if(h1nh2notend){
					SourceRow curRowh12(rh1nh2, headDes);
					while(h1nh2notend && ItemType(curRowh12.getGroup())==gh ){
						h1nh2->push_back(curRowh12.getHead());
						if ((h1nh2notend=rh1nh2->next())) {
							curRowh12.init(rh1nh2, headDes);
						}
					}
				}
			}


			if (!(gb<gh) && !(gb==gh)) {
				root->findRulesInTree(NULL, NULL, h1, h1nh2);
				newbody=0;newhead=1;
			} else if (gb<gh) {
				root->findRulesInTree(b1, b1nb2, NULL, NULL);
				newbody=1;newhead=0;
			} else if (gb==gh) {
				root->findRulesInTree(b1, b1nb2, h1, h1nh2);
				newbody=1;newhead=1;
			}

			if (!b1notend && !h1notend) notend=0;
		}
	}


	void DestrTree::adjustSupp(){
		MRLogPusher _("Starting the mining algorithm...");
	
		MRLog() << "Preparing the data structures..." << std::endl;
		insertRulesInStructure();
		MRLog() << "Done!" << std::endl;
		
		Connection connection;
		connection.setOutTableName(minerule->getParsedMinerule().tab_result);
		connection.useODBCConnection( MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
		connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), minerule->getParsedMinerule()));

		if (rb1!=NULL  &&  rh1!=NULL) {
				MRLog() << "Adjusting support..." << std::endl;
				adjustSuppRSet();
				MRLog() << "Done!" << std::endl;

				std::vector<ItemType> body;
				MRLog() << "Extracting rules..." << std::endl;
				getRoot()->extractRules(body, minerule->getParsedMinerule().sup, minerule->getParsedMinerule().conf, ngroups, &connection);
				MRLog() << "Done!" << std::endl;
		} else throw MineruleException (MR_ERROR_INTERNAL, " cannot create rules ");
	}


	size_t DestrTree::buildAttrStr(const ParsedMinerule::AttrVector& attr,
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
  
	std::string DestrTree::buildQry( const std::string& groupAttrStr,const std::string& attrStr,const std::string& constraints) const {
    
		return std::string("SELECT "+groupAttrStr+","+attrStr+" "
			"FROM "+minerule->getParsedMinerule().tab_source+" "+
			(constraints.size()>0 ?	"WHERE "+constraints+" " :"")
			+"ORDER BY "+groupAttrStr);
	}

  
	std::string DestrTree::buildQry1NotQry2( const std::string& groupAttrStr,const std::string& attrStr,const std::string& constraint1,	const std::string& constraint2) const {
    
		return std::string("SELECT "+groupAttrStr+","+attrStr+" "
			"FROM "+minerule->getParsedMinerule().tab_source+" "
			"WHERE "+constraint1+ (constraint2.empty() ? " " : " AND NOT("+constraint2+") ") +
			"ORDER BY "+groupAttrStr);
	}
	
	void DestrTree::prepareData() {
		MRLogPush("Building source information");

		MRLog() << "Separating the constraints in the HEAD and BODY parts..."<<std::endl;
		std::string q1BodyConstraints;
		std::string q1HeadConstraints;
		std::string q2BodyConstraints;
		std::string q2HeadConstraints;
		HeadBodyPredicatesSeparator::separate(minerule->getOptimizationInfo().minerule.mc!=NULL? minerule->getOptimizationInfo().minerule.mc->l_and:NULL, q1BodyConstraints, q1HeadConstraints);

		MRLog() << "Separating the constraints in the HEAD and BODY parts (query 2)..." << std::endl;

		HeadBodyPredicatesSeparator::separate(minerule->getParsedMinerule().mc->l_and, q2BodyConstraints, q2HeadConstraints);

		MRLog() << "Building db queries" << std::endl;
		size_t index;
		std::string groupAttr;
		std::string bodyAttr;
		std::string headAttr;
		index=buildAttrStr(minerule->getParsedMinerule().ga,0,groupAttr,bodyDes.groupElems );

		headDes.groupElems=bodyDes.groupElems;

		buildAttrStr(minerule->getParsedMinerule().ba,index,bodyAttr,bodyDes.bodyElems);
		buildAttrStr(minerule->getParsedMinerule().ha,index,headAttr,headDes.headElems);

		std::string bodyQry1 =buildQry( groupAttr,bodyAttr,q1BodyConstraints);
		std::string headQry1 =buildQry( groupAttr,headAttr,q1HeadConstraints);

		std::string bodyQry1NotQry2 = buildQry1NotQry2( groupAttr,bodyAttr,q1BodyConstraints,q2BodyConstraints);
		std::string headQry1NotQry2 = buildQry1NotQry2( groupAttr, headAttr, q1HeadConstraints, q2HeadConstraints);

		MRLog() << "Executing queries" << std::endl;
		mrdb::Connection* con = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();


		stateb1 = con->createStatement();
		MRDebug("bodyQry1:"+bodyQry1);
		rb1 = stateb1->executeQuery(bodyQry1);

		stateh1 = con->createStatement();
		MRDebug("headQry1:"+headQry1);
		rh1 = stateh1->executeQuery(headQry1);


		stateb1nb2 = con->createStatement();
		MRDebug("bodyQry1NotQry2:"+bodyQry1NotQry2);		
		rb1nb2 = stateb1nb2->executeQuery(bodyQry1NotQry2);

		stateh1nh2 = con->createStatement();
		MRDebug("headQry1NotQry2:"+headQry1NotQry2);		
		rh1nh2 = stateh1nh2->executeQuery(headQry1NotQry2);

		ngroups = PrepareDataUtils::evaluateTotGroups(minerule->getParsedMinerule());

		MRLogPop();
	}


	void DestrTree::execute()
	throw(MineruleException,mrdb::SQLException) {
		assert( minerule->getOptimizationInfo().relationship == OptimizedMinerule::Dominance );
		assert( minerule->getParsedMinerule().mc!=NULL && minerule->getParsedMinerule().mc->next==NULL);

		MRLogPush("This is the Context Dependent Destructive Mining Algorithm...");
		prepareData();
		adjustSupp();
		MRLogPop();
	}
} // namespace
