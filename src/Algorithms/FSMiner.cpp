//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Dino Ienco (dino.ienco@teledetection.fr)
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
#include "minerule/Algorithms/FSMiner.hpp"
#include "minerule/Algorithms/FSTree.hpp"
#include "minerule/Utils/MineruleLogs.hpp"
#include "minerule/Database/PrepareDataUtils.hpp"
#include <stdexcept>

//#include "sequence.hpp"

namespace minerule {

	void
	FSMiner::prepareData() {
		PrepareDataUtils pdu(minerule.getParsedMinerule(), this->sourceTableRequirements());
		const ParsedMinerule& pm = minerule.getParsedMinerule();
		std::string groupAttrList = pdu.buildAttrListDescription(pm.ga);
		std::string ordAttrList = pdu.buildAttrListDescription(pm.oa);

		std::string sqlQuery = 
			"SELECT "+ groupAttrList+","
			+ pdu.buildAttrListDescription(pm.ba) + ","
			+ ordAttrList + " "
			+ "FROM "+ pm.tab_source + " "
			+ "ORDER BY "+groupAttrList+","+ordAttrList;

		size_t last_elem;

		last_elem = rowDes.setgroupElems(1,pm.ga.size());

		rowDes.setBodyElems(last_elem+1, pm.ba.size());

		connection.useODBCConnection(MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
		connection.setOutTableName(minerule.getParsedMinerule().tab_result);
		connection.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);

	    throw std::runtime_error("Algorithm to be updated to use new createResiltTables API");
	    // connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), minerule.getParsedMinerule()));

		MRDebug() << "FSMiner query:" << sqlQuery.c_str() << std::endl;

		statement = connection.getODBCConnection()->prepareStatement(sqlQuery.c_str());

	}


	void 
	FSMiner::mineRules(){
		MRLogPush("This is FSMiner. Starting...");

		prepareData();
		FSTree* h=new FSTree(minerule);

		mrdb::ResultSet* rs = statement->executeQuery();
		MRLogPush("Building frequent bigram link table...");	
		h->createLinkSTable(rs, rowDes);
		MRLogPop();

		h->setThreshold(options.getSupport());
		MRLog() << "Support threshold is:" << h->getThreshold() << std::endl;

		MRLogPush("Building unfrequent bigram link table...");	
		h->createLinkNSTable();
		MRLogPop();

		delete rs;
		rs = statement->executeQuery();

		MRLogPush("Building FSTree...");
		h->construct_Tree(rs, rowDes);
		MRLogPop();

		delete rs;

		MRLogPush("Starting core mining...");
		h->mine();
		MRLogPop();

		MRLogPop();

		std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>::iterator res_it;
		std::map<FSTreeSequence,int,FSTreeSequence::less_sequence>* ris = h->getResult();
	//std::string s=""
		std::cout<<"ris->size "<<ris->size()<<std::endl;
		MRLogPush(std::string("contiguos frequent sequence find: ")+Converter(ris->size()).toString());
		for (res_it=ris->begin();res_it!=ris->end();res_it++){
			FSTreeSequence temp=(*res_it).first;
			MRLogPush(temp.toStdString());
		}


	// thrashing the thrashable
		delete ris;
		delete h;
		delete statement;
	}

	} // namespace
