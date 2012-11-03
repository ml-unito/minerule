#include <string.h>
#include <odbc++/resultset.h>
#include "Utils/MineruleOptions.h"
#include "Utils/SQLUtils.h"
#include "Optimizer/OptimizerCatalogue.h"
#include "Parsers/ParsedMinerule.h"
#include <memory>
#include <iterator>
#include <algorithm>

using namespace std;

namespace minerule {
  
  /*
    OptimizerCatalogue::OrderType OptimizerCatalogue::getOrderType(unsigned long origKey, 
    unsigned long refKey) const {
    OrderType result=None;

    odbc::Connection* connection =
    MineruleOptions::getSharedOptions().getOdbc_db().getConnection();
    odbc::Statement* state = connection->createStatement();

    string query =(string)
    "SELECT * "
    "FROM mr_eq_key_domains "
    "WHERE key_id_ref="+Converter((long)refKey).toString() +
    " AND key_id="+Converter((long)origKey).toString();
    
    odbc::ResultSet* rs = state->executeQuery(query);
    if(rs->next()) {
    if(rs->getString(3)=="e")
    result=Equal;
    else if( rs->getString(3)=="r" )
    result= Reversed;
    }
    
    delete rs;
    delete state;

    return result;
    }
  */

  bool OptimizerCatalogue::existsMinerule(const string& mrname) {
    odbc::Connection* conn = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    string query = (string)
      "SELECT * FROM mr_query WHERE query_name="+SQLUtils::quote(mrname);

    auto_ptr<odbc::Statement> stat(conn->createStatement());
    auto_ptr<odbc::ResultSet> rs(stat->executeQuery(query));

    return rs->next();
  }

  void OptimizerCatalogue::deleteMinerule(const string& mrname) 
    throw(MineruleException, odbc::SQLException) {
    vector<string> qryNames;

    MRLogPush("Deleting past minerules...");

    odbc::Connection* conn = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    string query = (string)
      "SELECT query_id "
      "FROM mr_query "
      "WHERE query_name="+SQLUtils::quote(mrname)+
      " OR tab_results_name="+SQLUtils::quote(mrname);

    auto_ptr<odbc::Statement> stat(conn->createStatement());
    odbc::ResultSet* rs=stat->executeQuery(query);

    while(rs->next()) {
      qryNames.push_back(rs->getString(1));
    }

    delete rs;

    MRLog() << "I found " << qryNames.size() 
	    << " minerules that need to be deleted" 
	    << endl;

    assert(qryNames.size()>0);
    if( qryNames.size()>1 && 
	!MineruleOptions::getSharedOptions().getSafety().getAllowCascadeDeletes() ) {
      throw MineruleException( MR_ERROR_SAFETY_PROBLEM, "Requested the delete of" 
			     " a minerule having dependent minerules, but option"
			     " safety::allowsCascadeDeletes is not set");
    }

    vector<string>::const_iterator it;
    for(it=qryNames.begin(); it!=qryNames.end(); it++) {
      MRLog() << "Deleting query: '" << *it << "'" << endl;

      string delQry = "DELETE FROM mr_query WHERE query_id="+SQLUtils::quote(*it);
      if(stat->execute(delQry)) {
	throw MineruleException( MR_ERROR_INTERNAL, "Cannot delete query "+*it );
      }
    }

    MRLog() << "Dropping tables connected to query named:" << mrname << endl;

    string dropQry = 
      "DROP TABLE IF EXISTS "+mrname+","+mrname+"_elems,"+mrname+"_tmpSource";
    
    stat->execute(dropQry);

    MRLogPop();
  }

  void
  OptimizerCatalogue::Catalogue::insertMapping(const string& table,
					       const KeyCols& cols,
					       int refKey,
					       OrderType orderType) throw(MineruleException)  {
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();
    
    try {
      auto_ptr<odbc::Statement> stat1( connection->createStatement() );

      string getKeyQuery = 
	string("SELECT "+getSchemaInfo("tab_cols_col_name")+" "+
	       "FROM "+getSchemaInfo("tab_cols")+" "+
	       "WHERE "+getSchemaInfo("tab_cols_key_id")+"=") + 
	                Converter((long)refKey).toString();

      auto_ptr<odbc::ResultSet> rs( stat1->executeQuery(getKeyQuery) );
      KeyCols refKeyCols;

      while(rs->next()) {
	refKeyCols.insert( rs->getString(1) );
      }

      /*      cout << "Inserting rule:" << endl;
      copy( cols.begin(),
	    cols.end(),
	    ostream_iterator< string >(cout, ","));
      cout << " -> ";
      copy( refKeyCols.begin(),
	    refKeyCols.end(),
	    ostream_iterator< string >( cout, ","));
	    cout << endl;*/
     
      //      OrderType orderType = getOrderType(origKey,refKey);
      // (*this)[table][cols] = pair<KeyCols,OrderType>(refKeyCols,orderType);
      pair<KeyCols, pair<KeyCols,OrderType> > insElem(cols,
		      pair<KeyCols,OrderType>(refKeyCols,orderType));
      (*this)[table].insert(insElem);

    } catch( odbc::SQLException& e ) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR,
			      string("Cannot access the db while "
				     "initializing a member of the OptimizerCatalogue,"
				     "the reason is:") + e.getMessage() );
    }
  }

  OptimizerCatalogue::OrderType 
  OptimizerCatalogue::Catalogue::stringToOrder(const string& str) {
    if( str == "r" )
      return Reversed;

    if( str == "e" )
      return Equal;

    return None;
  }

  void 
  OptimizerCatalogue::Catalogue::initialize() throw(MineruleException) {
    odbc::Connection* connection = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    try {
      auto_ptr<odbc::Statement> stat1( connection->createStatement() );
      string getCatalogueQuery = (string)
	"SELECT A."+getSchemaInfo("tab_main_tab_name")+
	      ",B."+getSchemaInfo("tab_cols_col_name")+
	      ",A."+getSchemaInfo("tab_main_rhs_key_id")+
	      ",A."+getSchemaInfo("tab_main_order_type")+" "+
	"FROM "+getSchemaInfo("tab_main")+" AS A,"+
	        getSchemaInfo("tab_cols")+" AS B "+
	"WHERE A."+getSchemaInfo("tab_main_lhs_key_id")+"=B."+
	           getSchemaInfo("tab_cols_key_id");
     
      auto_ptr<odbc::ResultSet> rs( stat1->executeQuery(getCatalogueQuery) );
      KeyCols origKey;
      unsigned long refKeyId=0;
      string curTable;
      OrderType orderType;


      // We start building the first of the two col l lists.
      // we accumulate the result in the set origKey. 

      if(rs->next()) {
	curTable = rs->getString(1);
	origKey.insert( rs->getString(2) );
	refKeyId= rs->getInt(3);
	orderType = stringToOrder(rs->getString(4));
      } else {
	return;
      }

      while( rs->next() ) {
	if(curTable!=rs->getString(1) || 
	   refKeyId!=(unsigned long)rs->getInt(3)) {
	  // here the current rule changed, in fact either the table
	  // or the refKeyId are different
	  insertMapping( curTable,origKey,refKeyId, orderType );
	  origKey.clear();
	  
	  curTable=rs->getString(1);
	  origKey.insert( rs->getString(2) );
	  refKeyId=rs->getInt(3);
	  orderType = stringToOrder(rs->getString(4));
	} else {
	  // here we continue to accumulate the columns in origKey
	  origKey.insert( rs->getString(2) );
	}
      } // end while

      assert( !origKey.empty() );
      insertMapping( curTable, origKey, refKeyId, orderType );
    } catch( odbc::SQLException& e ) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR,
			      string("Cannot access the db while "
				     "initializing a member of the OptimizerCatalogue,"
				     "the reason is:") + e.getMessage() );
    }
  }



  string OptimizerCatalogue::getNewAutoincrementValue(const string& tableName) 
    throw(odbc::SQLException,MineruleException) {
    odbc::Connection* connection = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();
    odbc::Statement* statement=NULL;
    odbc::ResultSet* rs=NULL;
    odbc::Statement* updateStatement=NULL;


    string idstr;

    try {
      statement=connection->createStatement();
      rs=statement->executeQuery("SELECT current_id FROM mr_autoincrement "
				 "WHERE table_name="+SQLUtils::quote(tableName));

      if(!rs->next())
	throw MineruleException(MR_ERROR_INSTALLATION_PROBLEM,
				"Cannot find the autoincrement value for table "+ tableName+
				" in Optimizer catalogue (i.e. in table:`mr_autoincrement'),"
				" please check your installation");

      size_t id = rs->getInt(1);
      idstr = Converter((long)++id).toString();
    
      delete rs;
      delete statement;
      rs=NULL;
      statement=NULL;


      updateStatement = connection->createStatement();
      updateStatement->execute("UPDATE mr_autoincrement SET current_id="+
			       idstr+" "
			       "WHERE table_name="+SQLUtils::quote(tableName));

      delete updateStatement;

    } catch (std::exception& e) {
      if( statement!=NULL ) delete statement;
      if( updateStatement!=NULL ) delete updateStatement;
      if( rs!=NULL) delete rs;

      throw;
    }

    return idstr;
  }

  string OptimizerCatalogue::addMineruleAttributeList(const ParsedMinerule::ListType& l) 
    throw (odbc::SQLException, MineruleException) {
    if( l.empty() )
      return "NULL";

    string id = getNewAutoincrementValue("mr_att_lists");
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    string query =
      "INSERT INTO mr_att_lists (att_list_id, col_name) VALUES ";
    
    ParsedMinerule::ListType::const_iterator it;
    for(it=l.begin(); it!=l.end(); it++) {
      if(it!=l.begin()) query+=",";
      
      query+="("+SQLUtils::quote(id)+","+SQLUtils::quote(*it)+")";
    }

    auto_ptr<odbc::Statement> statement(connection->createStatement());
    statement->execute(query);

    return id;
  }

  void OptimizerCatalogue::addMineruleResult(const OptimizerCatalogue::MineruleResultInfo& mri )
    throw(odbc::SQLException, MineruleException){

    odbc::Connection* connection = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    string galid = addMineruleAttributeList(mri.ga);
    string ralid = addMineruleAttributeList(mri.ra);
    string calid = addMineruleAttributeList(mri.ca);
    string qryid = getNewAutoincrementValue("mr_query");
    //std::cout<<"PIPPO"<<std::endl;
    auto_ptr<odbc::Statement> statement( connection->createStatement() );
    //std::cout<<"PLUTO"<<std::endl;
    statement->execute( "INSERT INTO mr_query "
			"(query_id, query_text, query_name, tab_results_name, source_tab_name,"
			"gal,ral,cal) VALUES ("+
			qryid+","+
			SQLUtils::quote(mri.getText())+","+
			SQLUtils::quote(mri.tab_result)+","+
			SQLUtils::quote(mri.resultset)+","+
			SQLUtils::quote(mri.tab_source)+","+
			galid+","+
			ralid+","+
			calid+")");
  
    std::cout<<"DINO"<<std::endl;
  }
  
  string OptimizerCatalogue::getResultsetName(const string& queryname) 
    throw(odbc::SQLException, MineruleException){
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    auto_ptr<odbc::Statement> statement(connection->createStatement());
    auto_ptr<odbc::ResultSet> rs(
				 statement->executeQuery("SELECT tab_results_name "
							 "FROM mr_query "
							 "WHERE query_name="+SQLUtils::quote(queryname)));
    if( !rs->next() )
      throw MineruleException(MR_ERROR_CATALOGUE_ERROR,
			      "Cannot find the query named:" + queryname +" "
			      "either there is some problem with the optimizer catalogue "
			      "or a wrong query name has been specified");
    
    return rs->getString(1);
  }

  void 
  OptimizerCatalogue::getMRQueryNames(vector<string>& nameVec) 
    throw(odbc::SQLException, MineruleException) {
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    auto_ptr<odbc::Statement> statement(connection->createStatement());
    auto_ptr<odbc::ResultSet> rs(
				 statement->executeQuery("SELECT query_name "
							 "FROM mr_query"));

    while( rs->next() ) {
      nameVec.push_back(rs->getString(1));
    }
  }

  void 
  OptimizerCatalogue::getMRQueryInfos(
			    vector<CatalogueInfo>& catInfoVec) 
    throw(odbc::SQLException, MineruleException) {
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    auto_ptr<odbc::Statement> statement(connection->createStatement());
    auto_ptr<odbc::ResultSet> rs(
	       statement->executeQuery("SELECT query_name,query_text, "
				       "  tab_results_name, source_tab_name "
				       "FROM mr_query"));

    while( rs->next() ) {
      CatalogueInfo info;
      info.qryName = rs->getString(1);
      info.qryText = rs->getString(2);
      info.resName = rs->getString(3);
      info.updateQrySize();

      catInfoVec.push_back(info);
    }
  }


  void 
  OptimizerCatalogue::getMRQueryInfo(const std::string& qryName,
				     CatalogueInfo& info) 
    throw(odbc::SQLException, MineruleException) {
    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    auto_ptr<odbc::Statement> statement(connection->createStatement());
    auto_ptr<odbc::ResultSet> rs(
	       statement->executeQuery("SELECT query_name,query_text, "
				       "  tab_results_name, source_tab_name "
				       "FROM mr_query "
				       "WHERE query_name='"+qryName+"'"));

    if( rs->next() ) {
      info.qryName = rs->getString(1);
      info.qryText = rs->getString(2);
      info.resName = rs->getString(3);
      info.updateQrySize();
    } else {
      throw MineruleException(MR_ERROR_CATALOGUE_ERROR,
			      "Cannot find the query named:" + qryName +" "
			      "Either there is some problem with the optimizer catalogue "
			      "or a wrong query name has been specified");
    }
  }

  void OptimizerCatalogue::getMRQueryResultIterator(const std::string& qryName,
						    QueryResult::Iterator& it,
						    double supp,
						    double conf) 
    throw(odbc::SQLException, MineruleException) {
    CatalogueInfo catInfo;
    getMRQueryInfo( qryName, catInfo );
    
    ParsedMinerule p;
    MineruleOptions::getSharedOptions().getLogStreamObj().disable();
    p.init(catInfo.qryText);
    MineruleOptions::getSharedOptions().getLogStreamObj().enable();
    if(supp<0)
      supp=p.sup;
    if(conf<0)
      conf=p.conf;

    it.init(catInfo.resName, supp, conf);
  }

  void
  CatalogueInfo::updateQrySize() throw (odbc::SQLException, MineruleException){
    ParsedMinerule p;
    MineruleOptions::getSharedOptions().getLogStreamObj().disable();
    p.init(qryText);
    MineruleOptions::getSharedOptions().getLogStreamObj().enable();
    
    string qry = 
      "SELECT count(*) "
      "FROM "+resName+" "+
      "WHERE supp>="+Converter(p.sup).toString()+
      " AND con>="+Converter(p.conf).toString();

    odbc::Connection* connection =
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();
    auto_ptr<odbc::Statement> statement(connection->createStatement());
    auto_ptr<odbc::ResultSet> rs(statement->executeQuery( qry ));
				 
    MRDebug() << "updateQrySize, the query is:" << qry << endl;

    if(rs->next()) {
      resSize = rs->getInt(1);
    } else {
      throw MineruleException(MR_ERROR_CATALOGUE_ERROR,
			      "CatalogueInfo::updateQrySize - cannot"
			      " retrieve the size of the result set");
    }
  }

  bool
  OptimizerCatalogue::isIDAttribute(const string& table,
		const ParsedMinerule::ListType& itemCols,
		const std::string& attribute) const throw(MineruleException)  {
    set<string> itemColsSet;
    copy( itemCols.begin(),
	  itemCols.end(),
	  insert_iterator< set<string> >(itemColsSet,itemColsSet.begin()));

    Catalogue::const_iterator cat_it = depFunCatalogue.find(table);
    if(cat_it==depFunCatalogue.end())
      return false;

    const CatalogueEntry& ce = cat_it->second;
    CatalogueEntry::const_iterator ce_it;
    for( ce_it=ce.begin(); ce_it!=ce.end(); ce_it++ ) {
      // if I can find the attribute in the rhs of the fd
      if(ce_it->second.first.find(attribute)!=ce_it->second.first.end() &&
	 // and I the item cols includes the lhs of the fd 
	 includes(itemColsSet.begin(),
		  itemColsSet.end(),
		  ce_it->first.begin(),
		  ce_it->first.end()))
	// then the attribute is item dependent
	return true;
    }
    
    return false;
  }
  

} // namespace




  