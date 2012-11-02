#include "PrepareDataUtils.h"  
#include "Utils/MineruleOptions.h"
#include <string>
#include <memory>

using namespace std;

namespace minerule {

string
  PrepareDataUtils::buildAttrListDescription( const ParsedMinerule::ListType& attrs,
					      const string& alias, 
					      bool addColAlias)  {
#define ADDCOLALIAS (addColAlias?(" AS " + alias+*it):"")

    ParsedMinerule::ListType::const_iterator it;
    string result;
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
  PrepareDataUtils::buildAttrListAlias( const ParsedMinerule::ListType& attrs,
					      const string& alias, 
					      bool addColAlias)  {
#define ADDCOLALIAS (addColAlias?(alias+*it):"")

    ParsedMinerule::ListType::const_iterator it;
    string result;
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

  string
  PrepareDataUtils::buildAttrListEquiJoin( const string& alias1,
					    const string& alias2) const {
  
    ParsedMinerule::ListType::const_iterator it;
    string result;
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
      MRLog() << "filtering query:" << result << endl;
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


  void
  PrepareDataUtils::buildSimpleSourceTableQuery(
						string& queryText,
						HeadBodySourceRowDescription& rowDes) const {
    queryText = "SELECT ";
    queryText += buildAttrListDescription(mr.getParsedMinerule().ga);
    queryText += "," + buildAttrListDescription(mr.getParsedMinerule().ba);
    queryText += " FROM "+mr.getParsedMinerule().tab_source;
#ifdef MRUSERWARNING
#warning Bisogna aggiungere la parte di gestione dei filtri...
#endif
    if( miningAlgo.needsGidSortedSourceTable() )
      queryText += " ORDER BY "+buildAttrListDescription(mr.getParsedMinerule().ga);
					      
    unsigned int lastElem;
    lastElem= rowDes.setGroupBodyElems(1,mr.getParsedMinerule().ga.size());
    lastElem= rowDes.setBodyElems(lastElem+1,mr.getParsedMinerule().ba.size());
  }

  string
  PrepareDataUtils::buildAndList(const list_AND_node* l) const {
    const list_AND_node* current = l;
    string result;

    assert( current!=NULL );
    assert( current->sp!=NULL );

    result = string(current->sp->val1) + string(current->sp->op) + string(current->sp->val2);
    current = current->next;

    while( current!=NULL ) {
      assert(current->sp!=NULL );
      result += " AND "+ string(current->sp->val1) + string(current->sp->op) + string(current->sp->val2);
      current = current->next;
    }

    return result;
  }

  string
  PrepareDataUtils::buildConditionFilter(const list_OR_node* current) const {
    string result;

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
  PrepareDataUtils::dropTableIfExists(odbc::Connection* conn, const string& tname)  {
    std::auto_ptr<odbc::Statement> state(conn->createStatement());
    try {
      state->executeQuery("DROP TABLE "+tname);
    } catch(odbc::SQLException& s) {
      // simply ignore any error (i.e., table not exists error, all the others
      // will be handled by the forthcoming queries).
    }
  }

  string
  PrepareDataUtils::createSourceTable() const {
    string aliasA, aliasB;
    string queryText;
    assert( !(mr.getParsedMinerule().ga.empty() || mr.getParsedMinerule().ba.empty() || mr.getParsedMinerule().ha.empty()) );
    aliasA = "BODY";
    aliasB = "HEAD";

    string groupAttrList =  buildAttrListDescription(mr.getParsedMinerule().ga, aliasA,true);

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
  
    string miningCond = buildConditionFilter(mr.getParsedMinerule().mc);
    if(miningCond!="") {
      queryText += " AND (" + miningCond +")";
    }

    string clusterCond = buildConditionFilter(mr.getParsedMinerule().cc);
    if(clusterCond!="") {
      queryText += " AND (" + clusterCond +")";
    }
#ifdef MRUSERWARNING
#warning Bisogna aggiungere la parte di gestione dei filtri...
#endif

    if( miningAlgo.needsGidSortedSourceTable() )
      queryText += " ORDER BY "+buildAttrListDescription(mr.getParsedMinerule().ga, aliasA,false);
    
    // FIXME - on postgres the create table syntax is like:
	// 		create table <table name> as select ...
	// on mysql is
	// 		create table <table name> select ... (i.e., without the 'as' keyword)
	// momentarily we use the postgres syntax, but one fix that accomodates both
	// should be implemented.
    string clusteredTable = mr.getParsedMinerule().tab_result+"_tmpSource";
    string createQuery = "CREATE TABLE  "+clusteredTable+ " AS "+queryText + "; ";
	string createIndexQuery = "CREATE INDEX "+clusteredTable+"_index " + " ON " + clusteredTable + " ("+buildAttrListAlias(mr.getParsedMinerule().ga,aliasA,true)+");";

    odbc::Connection* conn = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    dropTableIfExists(conn, clusteredTable);

    //std::auto_ptr<odbc::Statement> state(conn->createStatement());
    odbc::Statement* state = conn->createStatement();

    MRLog() << "Creating clustered table... " << endl;
    MRDebug() << "Creating mining table, the query is:" 
	      << createQuery << endl;
    state->execute(createQuery);
	state->execute(createIndexQuery);
	
    MRLog() << "DONE..." << endl;
    delete state;

    return  clusteredTable;
  }


  void
  PrepareDataUtils::buildExtendedSourceTableQuery(
						  string& queryText,
						  HeadBodySourceRowDescription& rowDes) const  {
    string tableName = createSourceTable();

    queryText = "SELECT * FROM "+tableName;
					      
    unsigned int lastElem;
    lastElem= rowDes.setGroupBodyElems(1,mr.getParsedMinerule().ga.size());
    lastElem= rowDes.setClusterBodyElems(lastElem+1,mr.getParsedMinerule().ca.size());
    lastElem= rowDes.setBodyElems(lastElem+1,mr.getParsedMinerule().ba.size());
    lastElem= rowDes.setClusterHeadElems(lastElem+1,mr.getParsedMinerule().ca.size());
    lastElem= rowDes.setHeadElems(lastElem+1, mr.getParsedMinerule().ha.size());
  }


  void
  PrepareDataUtils::buildSourceTableQuery(
					   string& queryText,
					   HeadBodySourceRowDescription& rowDes) const {
    if(miningAlgo.needsCrossProductOfSourceTable() ) {
      buildExtendedSourceTableQuery(queryText,rowDes);
    } else {
      buildSimpleSourceTableQuery(queryText,rowDes);
    }
  }


  size_t PrepareDataUtils::evaluateTotGroups(const ParsedMinerule& pmr) throw(MineruleException, odbc::SQLException) {
    MRLogPush("Evaluating Number of groups...");
    string qry=
      "SELECT count(distinct " + buildAttrListDescription(pmr.ga) +") "
      "FROM " + pmr.tab_source;
    
    odbc::Connection* conn = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    std::auto_ptr<odbc::Statement> state(conn->createStatement());
    std::auto_ptr<odbc::ResultSet> rs(state->executeQuery(qry));

    MRLogPop();

    if(!rs->next())
      throw MineruleException(MR_ERROR_DATABASE_ERROR,
			      "Cannot count the number of groups in the source table");
    
    return rs->getInt(1);
  }

} // namespace
