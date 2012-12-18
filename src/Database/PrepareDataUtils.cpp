#include "minerule/Database/PrepareDataUtils.h"  
#include "minerule/Utils/MineruleOptions.h"
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

	std::string
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
		it=mr.ga.begin();

    // *** Filtering out rows not having the same gid ***
    // the first element should not have a leading ","
		assert(it!=mr.ga.end());
		result=alias1+"."+*it+"="+alias2+"."+*it;
		it++;
  
		for( ;it!=mr.ga.end(); it++ ) {
			result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
		}

    // *** Filtering out rows having the same cid and the same item

    // *** if body!=head there will be no tautologies and hence we
    // *** cannot actually filter out anything
		if( !mr.body_coincident_head ) {
			MRLog() << "filtering query:" << result << std::endl;
			return result;
		}

		result+=" AND NOT (";
		if( !mr.ca.empty() ) {
			it=mr.ca.begin();
			assert(it!=mr.ca.end());
			result+=alias1+"."+*it+"="+alias2+"."+*it;
			it++;
  
			for( ;it!=mr.ca.end(); it++ ) {
				result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
			}
     
			result+=" AND ";
		}
  
		it=mr.ba.begin();
		assert(it!=mr.ba.end());
		result+=alias1+"."+*it+"="+alias2+"."+*it;
		it++;
  
		for( ;it!=mr.ba.end(); it++ ) {
			result+=" AND "+ alias1+"."+*it+"="+alias2+"."+*it;
		}
  
		result +=")";

		return result; 
	}


	

	std::string
	PrepareDataUtils::buildBodyTableQuery(SourceRowColumnIds& rowDes, const std::string& body_mining_condition) const {
		std::string queryText;		
		
		queryText = "SELECT ";
		queryText += buildAttrListDescription(mr.ga);
		queryText += "," + buildAttrListDescription(mr.ba);
		queryText += " FROM "+mr.tab_source;
		if(!body_mining_condition.empty())
			queryText += " WHERE "+body_mining_condition;

		if( sourceTableRequirements.sortedGids() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.ga);
					      
		unsigned int lastElem;
		lastElem= rowDes.setgroupElems(1,mr.ga.size());
		rowDes.setBodyElems(lastElem+1,mr.ba.size());
		
		return queryText;
	}
	
	std::string
	PrepareDataUtils::buildHeadTableQuery(SourceRowColumnIds& rowDes, const std::string& head_mining_condition) const {
		std::string queryText;		
		
		queryText = "SELECT ";
		queryText += buildAttrListDescription(mr.ga);
		queryText += "," + buildAttrListDescription(mr.ha);
		queryText += " FROM "+mr.tab_source;
		
		if(!head_mining_condition.empty())
			queryText += " WHERE "+head_mining_condition;

		if( sourceTableRequirements.sortedGids() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.ga);
					      
		unsigned int lastElem;
		lastElem= rowDes.setgroupElems(1,mr.ga.size());
		rowDes.setHeadElems(lastElem+1,mr.ha.size());
		
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
		state->execute("DROP TABLE IF EXISTS "+tname);
	}

	std::string
	PrepareDataUtils::createSourceTable() const {
		std::string aliasA, aliasB;
		std::string queryText;
	  
		assert( !(mr.ga.empty() || mr.ba.empty() || mr.ha.empty()) );

		aliasA = "BODY";
		aliasB = "HEAD";

		std::string groupAttrList =  buildAttrListDescription(mr.ga, aliasA,true);

		queryText = "SELECT ";
		queryText += groupAttrList;
		if( !mr.ca.empty() )
			queryText += "," + buildAttrListDescription(mr.ca, aliasA,true);
		queryText += "," + buildAttrListDescription(mr.ba, aliasA,true);
		if( !mr.ca.empty() )
			queryText += "," + buildAttrListDescription(mr.ca, aliasB,true);
		queryText += "," + buildAttrListDescription(mr.ha, aliasB,true);

		queryText += " FROM "+mr.tab_source+" AS "+aliasA;
		queryText += ","+mr.tab_source+" AS "+aliasB;

		queryText += " WHERE " + buildAttrListEquiJoin( aliasA, aliasB);
  
		std::string miningCond = buildConditionFilter(mr.mc);
		if(miningCond!="") {
			queryText += " AND (" + miningCond +")";
		}

		std::string clusterCond = buildConditionFilter(mr.cc);
		if(clusterCond!="") {
			queryText += " AND (" + clusterCond +")";
		}

		if( sourceTableRequirements.sortedGids() )
			queryText += " ORDER BY "+buildAttrListDescription(mr.ga, aliasA,false);
    
		std::string clusteredTable 	 = mr.tab_result+"_tmpSource";
		std::string createQuery 	 = "CREATE TABLE  "+clusteredTable+ " AS "+queryText + "; ";
		std::string createIndexQuery = "CREATE INDEX "+clusteredTable+"_index " + " ON " + clusteredTable + " ("+buildAttrListAlias(mr.ga,aliasA,true)+");";

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
		lastElem= rowDes.setgroupElems(1,mr.ga.size());
		lastElem= rowDes.setClusterBodyElems(lastElem+1,mr.ca.size());
		lastElem= rowDes.setBodyElems(lastElem+1,mr.ba.size());
		lastElem= rowDes.setClusterHeadElems(lastElem+1,mr.ca.size());
		rowDes.setHeadElems(lastElem+1, mr.ha.size());
		
		return queryText;
	}


	std::string PrepareDataUtils::buildSourceTableQuery(SourceRowColumnIds& rowDes) const {
		if(sourceTableRequirements.crossProduct() ) {
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
