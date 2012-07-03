#include "FSMiner.h"
#include "FSTree.h"
#include "Utils/MineruleLogs.h"
#include "Algorithms/PrepareDataUtils.h"

//#include "sequence.h"

namespace minerule {

	void
	FSMiner::prepareData() {
		PrepareDataUtils pdu(minerule, *this);
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

		last_elem = rowDes.setGroupBodyElems(1,pm.ga.size());

		rowDes.setBodyElems(last_elem+1, pm.ba.size());

		coreConn.useConnection(MineruleOptions::getSharedOptions().getOdbc_db().getConnection());
		coreConn.setOutTableName(minerule.getParsedMinerule().tab_result);
		coreConn.setBodyCardinalities(minerule.getParsedMinerule().bodyCardinalities);
		coreConn.create_db_rule(0);

		MRDebug() << "FSMiner query:" << sqlQuery.c_str() << endl;

		statement = coreConn.getConnection()->prepareStatement(sqlQuery.c_str());

	}


	void 
	FSMiner::execute(){
		MRLogPush("This is FSMiner. Starting...");

		prepareData();
		FSTree* h=new FSTree(minerule);

		odbc::ResultSet* rs = statement->executeQuery();
		MRLogPush("Building frequent bigram link table...");	
		h->createLinkSTable(rs, rowDes);
		MRLogPop();

		h->setThreshold(options.getSupport());
		MRLog() << "Support threshold is:" << h->getThreshold() << endl;

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

		map<FSTreeSequence,int,FSTreeSequence::less_sequence>::iterator res_it;
		map<FSTreeSequence,int,FSTreeSequence::less_sequence>* ris = h->getResult();
	//string s=""
		cout<<"ris->size "<<ris->size()<<endl;
		MRLogPush("contiguos frequent sequence find: "+ris->size());
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
