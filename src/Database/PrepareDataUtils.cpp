#include "Database/PrepareDataUtils.h"  
#include "Utils/MineruleOptions.h"
#include <string>
#include <memory>



namespace minerule {

	std::string PrepareDataUtils::buildAttrListDescription( const ParsedMinerule::AttrVector& attrs, const std::string& alias, bool addColAlias)  {

		#define ADDCOLALIAS (addColAlias?(" AS " + alias+*it):"")

		ParsedMinerule::AttrVector::const_iterator it;
		std::string result;
		it=attrs.begin();

    // the first element should not have a leading ","
		assert(it!=attrs.end());
		if(alias.length()!=0)
			result=alias+"."+*it + ADDCOLALIAS;
		else
			result=*it;

		it++;
  
		for( ;it!=attrs.end(); it++ ) {
			if(alias.length()!=0)
				result += ","+alias+"."+*it + ADDCOLALIAS;
			else
				result+=","+*it;
		}

		#undef ADDCOLALIAS

		return result;
	}

	string
	PrepareDataUtils::buildAttrListAlias( const ParsedMinerule::AttrVector& attrs,	const std::string& alias, bool addColAlias)  {
		#define ADDCOLALIAS (addColAlias?(alias+*it):"")

		ParsedMinerule::AttrVector::const_iterator it;
		std::string result;
		it=attrs.begin();

    // the first element should not have a leading ","
		assert(it!=attrs.end());
		if(alias.length()!=0)
			result=ADDCOLALIAS;
		else
			result=*it;

		it++;
  
		for( ;it!=attrs.end(); it++ ) {
			if(alias.length()!=0)
				result += ","+ADDCOLALIAS;
			else
				result+=","+*it;
		}

		#undef ADDCOLALIAS

		return result;
	}

	std::string PrepareDataUtils::buildAttrListEquiJoin( const std::string& alias1, const std::string& alias2) const {
  
		ParsedMinerule::AttrVector::const_iterator it;
		std::string result;
		it=mr.getParsedMinerule().ga.begin();

    // *** Filtering out rows not having the same gid ***
    // the first element should not have a leading ","
		assert(it!=mr.getParsedMinerule().ga.end());
		result=alias1+"."+*it+"="+alias2+"."+*it;
		it++;
  
		for( ;it!=mr.getParsedMinerule().ga.end(); it++ ) {
			result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
		}

    // *** Filtering out rows having the same cid and the same item

    // *** if body!=head there will be no tautologies and hence we
    // *** cannot actually filter out anything
		if( !mr.getParsedMinerule().body_coincident_head ) {
			MRLog() << "filtering query:" << result << std::endl;
			return result;
		}

		result+=" AND NOT (";
		if( !mr.getParsedMinerule().ca.empty() ) {
			it=mr.getParsedMinerule().ca.begin();
			assert(it!=mr.getParsedMinerule().ca.end());
			result+=alias1+"."+*it+"="+alias2+"."+*it;
			it++;
  
			for( ;it!=mr.getParsedMinerule().ca.end(); it++ ) {
				result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
			}
     
			result+=" AND ";
		}
  
		it=mr.getParsedMinerule().ba.begin();
		assert(it!=mr.getParsedMinerule().ba.end());
		result+=alias1+"."+*it+"="+alias2+"."+*it;
		it++;
  
		for( ;it!=mr.getParsedMinerule().ba.end(); it++ ) {
			result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
		}
  
		result +=")";

		return result; 
	}


	

	std::string
	PrepareDataUtils::buildBodyTableQuery(SourceRowColumnIds& rowDes, const std::string& body_mining_condition) const {
		std::string queryText;		
		
		queryText = "SELECT ";
		queryText += buildAttrListDescription(mr.getParsedMinerule().ga);
		queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ba);
		queryText += " FROM "+mr.getParsedMinerule().tab_source;
		queryText += " WHERE "+body_mining_condition;

		if( miningAlgo.needsGidSortedSourceTable() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.getParsedMinerule().ga);
					      
		unsigned int lastElem;
		lastElem= rowDes.setgroupElems(1,mr.getParsedMinerule().ga.size());
		rowDes.setBodyElems(lastElem+1,mr.getParsedMinerule().ba.size());
		
		return queryText;
	}
	
	std::string
	PrepareDataUtils::buildHeadTableQuery(SourceRowColumnIds& rowDes, const std::string& head_mining_condition) const {
		std::string queryText;		
		
		queryText = "SELECT ";
		queryText += buildAttrListDescription(mr.getParsedMinerule().ga);
		queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ha);
		queryText += " FROM "+mr.getParsedMinerule().tab_source;
		queryText += " WHERE "+head_mining_condition;

		if( miningAlgo.needsGidSortedSourceTable() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.getParsedMinerule().ga);
					      
		unsigned int lastElem;
		lastElem= rowDes.setgroupElems(1,mr.getParsedMinerule().ga.size());
		rowDes.setHeadElems(lastElem+1,mr.getParsedMinerule().ha.size());
		
		return queryText;
	}

	std::string PrepareDataUtils::buildAndList(const list_AND_node* l) const {
		const list_AND_node* current = l;
		std::string result;

		assert( current!=NULL );
		assert( current->sp!=NULL );

		result = std::string(current->sp->val1) + std::string(current->sp->op) + std::string(current->sp->val2);
		current = current->next;

		while( current!=NULL ) {
			assert(current->sp!=NULL );
			result += " AND "+ std::string(current->sp->val1) + std::string(current->sp->op) + std::string(current->sp->val2);
			current = current->next;
		}

		return result;
	}

	std::string
	PrepareDataUtils::buildConditionFilter(const list_OR_node* current) const {
		std::string result;

		if( current==NULL ) 
			return "";

		result = "("+ buildAndList(current->l_and)+")";
		current = current->next;

		while( current!=NULL ) {
			result += " OR (" + buildAndList(current->l_and) + ")";
			current = current->next;
		}

		return result;
	}

	void
	PrepareDataUtils::dropTableIfExists(odbc::Connection* conn, const std::string& tname)  {
		std::auto_ptr<odbc::Statement> state(conn->createStatement());
		state->executeQuery("DROP TABLE IF EXISTS "+tname);
	}

	string
	PrepareDataUtils::createSourceTable() const {
		std::string aliasA, aliasB;
		std::string queryText;
	  
		assert( !(mr.getParsedMinerule().ga.empty() || mr.getParsedMinerule().ba.empty() || mr.getParsedMinerule().ha.empty()) );

		aliasA = "BODY";
		aliasB = "HEAD";

		std::string groupAttrList =  buildAttrListDescription(mr.getParsedMinerule().ga, aliasA,true);

		queryText = "SELECT ";
		queryText += groupAttrList;
		if( !mr.getParsedMinerule().ca.empty() )
			queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ca, aliasA,true);
		queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ba, aliasA,true);
		if( !mr.getParsedMinerule().ca.empty() )
			queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ca, aliasB,true);
		queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ha, aliasB,true);

		queryText += " FROM "+mr.getParsedMinerule().tab_source+" AS "+aliasA;
		queryText += ","+mr.getParsedMinerule().tab_source+" AS "+aliasB;

		queryText += " WHERE " + buildAttrListEquiJoin( aliasA, aliasB);
  
		std::string miningCond = buildConditionFilter(mr.getParsedMinerule().mc);
		if(miningCond!="") {
			queryText += " AND (" + miningCond +")";
		}

		std::string clusterCond = buildConditionFilter(mr.getParsedMinerule().cc);
		if(clusterCond!="") {
			queryText += " AND (" + clusterCond +")";
		}

		if( miningAlgo.needsGidSortedSourceTable() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.getParsedMinerule().ga, aliasA,false);
    
		std::string clusteredTable 	 = mr.getParsedMinerule().tab_result+"_tmpSource";
		std::string createQuery 	 = "CREATE TABLE  "+clusteredTable+ " AS "+queryText + "; ";
		std::string createIndexQuery = "CREATE INDEX "+clusteredTable+"_index " + " ON " + clusteredTable + " ("+buildAttrListAlias(mr.getParsedMinerule().ga,aliasA,true)+");";

		odbc::Connection* conn = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		dropTableIfExists(conn, clusteredTable);

		odbc::Statement* state = conn->createStatement();

		MRLog() << "Creating clustered table... " << std::endl;
		MRDebug("Creating mining table, the query is:" + createQuery);

		state->execute(createQuery);
		state->execute(createIndexQuery);
	
		MRLog() << "DONE..." << std::endl;

		delete state;

		return  clusteredTable;
	}


	std::string PrepareDataUtils::buildExtendedSourceTableQuery(SourceRowColumnIds& rowDes) const  {
		std::string tableName = createSourceTable();

		std::string queryText = "SELECT * FROM "+tableName;
					      
		unsigned int lastElem;
		lastElem= rowDes.setgroupElems(1,mr.getParsedMinerule().ga.size());
		lastElem= rowDes.setClusterBodyElems(lastElem+1,mr.getParsedMinerule().ca.size());
		lastElem= rowDes.setBodyElems(lastElem+1,mr.getParsedMinerule().ba.size());
		lastElem= rowDes.setClusterHeadElems(lastElem+1,mr.getParsedMinerule().ca.size());
		rowDes.setHeadElems(lastElem+1, mr.getParsedMinerule().ha.size());
		
		return queryText;
	}


	std::string PrepareDataUtils::buildSourceTableQuery(SourceRowColumnIds& rowDes) const {
		if(miningAlgo.needsCrossProductOfSourceTable() ) {
			return buildExtendedSourceTableQuery(rowDes);
		} else {
			throw MineruleException(MR_ERROR_INTERNAL, "Check for consistency with previous implementation");
			return buildBodyTableQuery(rowDes,"");
		}
	}


	size_t PrepareDataUtils::evaluateTotGroups(const ParsedMinerule& pmr) throw(MineruleException, odbc::SQLException) {
		MRLogPush("Evaluating Number of groups...");
		std::string qry=
			"SELECT count(distinct " + buildAttrListDescription(pmr.ga) +") "
				"FROM " + pmr.tab_source;
    
		odbc::Connection* conn = 
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> state(conn->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(state->executeQuery(qry));

		MRLogPop();

		if(!rs->next())
			throw MineruleException(MR_ERROR_DATABASE_ERROR, "Cannot count the number of groups in the source table");
    
		return rs->getInt(1);
	}

} // namespace