#include <string.h>
#include <odbc++/resultset.h>
#include "Utils/MineruleOptions.h"
#include "Utils/SQLUtils.h"
#include "Optimizer/OptimizerCatalogue.h"
#include "Parsers/ParsedMinerule.h"
#include "Database/Connection.h"
#include <memory>
#include <iterator>
#include <algorithm>
#include <iomanip>

namespace minerule {

	bool OptimizerCatalogue::existsMinerule(const std::string& mrname) {
		odbc::Connection* conn = 
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::string query = (std::string)
			"SELECT * FROM mr_query WHERE query_name="+SQLUtils::quote(mrname);

		std::auto_ptr<odbc::Statement> stat(conn->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(stat->executeQuery(query));

		return rs->next();
	}

	void OptimizerCatalogue::deleteMinerule(const std::string& mrname) 
	throw(MineruleException, odbc::SQLException) {
		std::vector<std::string> qryNames;

		MRLogPusher _("Deleting past minerules...");

		Connection conn;
		conn.useODBCConnection(MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
		conn.setOutTableName(mrname);

		std::string query = (std::string)
			"SELECT query_id "
				"FROM mr_query "
					"WHERE query_name="+SQLUtils::quote(mrname)+
		" OR tab_results_name="+SQLUtils::quote(mrname);

		std::auto_ptr<odbc::Statement> stat(conn.getODBCConnection()->createStatement());
		odbc::ResultSet* rs=stat->executeQuery(query);

		while(rs->next()) {
			qryNames.push_back(rs->getString(1));
		}

		delete rs;

		MRLog() << "I found " << qryNames.size() 
			<< " minerules that need to be deleted" 
				<< std::endl;

		assert(qryNames.size()>0);
		if( qryNames.size()>1 && !MineruleOptions::getSharedOptions().getSafety().getAllowCascadeDeletes() ) {
			throw MineruleException( MR_ERROR_SAFETY_PROBLEM, "Requested the delete of" 
				" a minerule having dependent minerules, but option"
					" safety::allowsCascadeDeletes is not set");
		}

		std::vector<std::string>::const_iterator it;
		for(it=qryNames.begin(); it!=qryNames.end(); it++) {
			MRLog() << "Deleting query: '" << *it << "'" << std::endl;

			std::string delQry = "DELETE FROM mr_query WHERE query_id="+SQLUtils::quote(*it);
			if(stat->execute(delQry)) {
				throw MineruleException( MR_ERROR_INTERNAL, "Cannot delete query "+*it );
			}
		}

		MRLog() << "Dropping tables connected to query named:" << mrname << std::endl;
		conn.deleteDestTables();
	}

	void
		OptimizerCatalogue::Catalogue::insertMapping(const std::string& table,
			const KeyCols& cols,
				int refKey,
	OrderType orderType) throw(MineruleException)  {
		odbc::Connection* connection =
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
    
		try {
			std::auto_ptr<odbc::Statement> stat1( connection->createStatement() );

			std::string getKeyQuery = 
				std::string("SELECT "+getSchemaInfo("tab_cols_col_name")+" "+
			"FROM "+getSchemaInfo("tab_cols")+" "+
			"WHERE "+getSchemaInfo("tab_cols_key_id")+"=") + 
			Converter((long)refKey).toString();

			std::auto_ptr<odbc::ResultSet> rs( stat1->executeQuery(getKeyQuery) );
			KeyCols refKeyCols;

			while(rs->next()) {
				refKeyCols.insert( rs->getString(1) );
			}
     
      //      OrderType orderType = getOrderType(origKey,refKey);
      // (*this)[table][cols] = pair<KeyCols,OrderType>(refKeyCols,orderType);
			std::pair<KeyCols, std::pair<KeyCols,OrderType> > insElem(cols,std::pair<KeyCols,OrderType>(refKeyCols,orderType));
			(*this)[table].insert(insElem);

		} catch( odbc::SQLException& e ) {
			throw MineruleException(MR_ERROR_DATABASE_ERROR,
				std::string("Cannot access the db while "
					"initializing a member of the OptimizerCatalogue,"
						"the reason is:") + e.getMessage() );
		}
	}

	OptimizerCatalogue::OrderType 
	OptimizerCatalogue::Catalogue::stringToOrder(const std::string& str) {
		if( str == "r" )
			return Reversed;

		if( str == "e" )
			return Equal;

		return None;
	}

	void 
	OptimizerCatalogue::Catalogue::initialize() throw(MineruleException) {
		odbc::Connection* connection = 
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		try {
			std::auto_ptr<odbc::Statement> stat1( connection->createStatement() );
			std::string getCatalogueQuery = (std::string)
				"SELECT A."+getSchemaInfo("tab_main_tab_name")+
			",B."+getSchemaInfo("tab_cols_col_name")+
			",A."+getSchemaInfo("tab_main_rhs_key_id")+
			",A."+getSchemaInfo("tab_main_order_type")+" "+
			"FROM "+getSchemaInfo("tab_main")+" AS A,"+
			getSchemaInfo("tab_cols")+" AS B "+
			"WHERE A."+getSchemaInfo("tab_main_lhs_key_id")+"=B."+
			getSchemaInfo("tab_cols_key_id");
     
			std::auto_ptr<odbc::ResultSet> rs( stat1->executeQuery(getCatalogueQuery) );
			KeyCols origKey;
			unsigned long refKeyId=0;
			std::string curTable;
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
				if(curTable!=rs->getString(1) || refKeyId!=(unsigned long)rs->getInt(3)) {
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
			throw MineruleException(MR_ERROR_DATABASE_ERROR, std::string("Cannot access the db while "
				"initializing a member of the OptimizerCatalogue, the reason is:") + e.getMessage() );
		}
	}



	std::string OptimizerCatalogue::getNewAutoincrementValue(const std::string& tableName)  throw(odbc::SQLException,MineruleException) {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::string idstr;

		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(statement->executeQuery("SELECT current_id FROM mr_autoincrement WHERE table_name="+SQLUtils::quote(tableName)));

		if(!rs->next())
			throw MineruleException(MR_ERROR_INSTALLATION_PROBLEM,
				"Cannot find the autoincrement value for table "+ tableName+
				" in Optimizer catalogue (i.e. in table:`mr_autoincrement'),"
				" please check your installation");

		size_t id = rs->getInt(1);
		idstr = Converter((long)++id).toString();
    
		std::auto_ptr<odbc::Statement> updateStatement(connection->createStatement());
		updateStatement->execute("UPDATE mr_autoincrement SET current_id="+
			idstr+" WHERE table_name="+SQLUtils::quote(tableName));

		return idstr;
	}

	std::string OptimizerCatalogue::addMineruleAttributeList(const ParsedMinerule::AttrVector& l) throw (odbc::SQLException, MineruleException) {
		if( l.empty() )
			return "NULL";

		std::string id = getNewAutoincrementValue("mr_att_lists");
		odbc::Connection* connection =
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::string query =
			"INSERT INTO mr_att_lists (att_list_id, col_name) VALUES ";
    
		ParsedMinerule::AttrVector::const_iterator it;
		for(it=l.begin(); it!=l.end(); it++) {
			if(it!=l.begin()) query+=",";
      
			query+="("+SQLUtils::quote(id)+","+SQLUtils::quote(*it)+")";
		}

		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		statement->execute(query);

		return id;
	}
  
	bool OptimizerCatalogue::checkInstallation() {
		odbc::Connection* connection =  MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		odbc::DatabaseMetaData* meta = connection->getMetaData();
		// The set of table should be retrieved with ->getTables(), it appears that the postgres odbc driver
		// has some problem with this function (our tests receives an empty set no matter the parameters).
		// The following uses getColumns instead, it is slower (it retrieves all columns for all tables), but
		// is works.
		odbc::ResultSet* rs = meta->getColumns("","","mr_%", "");
		
		typedef std::pair<std::string, bool> TableInfo;
		std::vector<TableInfo> tables;
		tables.push_back(TableInfo("mr_query", false));
		tables.push_back(TableInfo("mr_att_lists", false));
		tables.push_back(TableInfo("mr_eq_keys", false));
		tables.push_back(TableInfo("mr_eq_keys_col", false));
		tables.push_back(TableInfo("mr_dep_fun", false));
		tables.push_back(TableInfo("mr_dep_fun_col", false));
		tables.push_back(TableInfo("mr_autoincrement", false));
		
		rs->next();
		while(!rs->isAfterLast()) {
			std::string currentTable = rs->getString("TABLE_NAME");
			std::vector<TableInfo>::iterator it = find(tables.begin(), tables.end(), std::pair<std::string,bool>(currentTable,false) );
			
			if(it!=tables.end()) {
				MRLog() << "Table: " << std::left << std::setw(50) << currentTable << StringUtils::toGreen(" OK")<< std::endl;
				it->second = true;
			}
			rs->next();
		}
			  
		delete rs;

		bool allTablePresents = true;
		for(std::vector<TableInfo>::iterator it=tables.begin(); it!=tables.end();++it) {
			allTablePresents &= it->second;
			if( !it->second ) {
				MRLog() << "Table: " << std::left << std::setw(50) << it->first << StringUtils::toRed(" MISSING") << std::endl;				
			}
		}
		
		return allTablePresents;
	}
	
	
	void OptimizerCatalogue::install() throw(MineruleException, odbc::SQLException) {		
		CatalogueInstaller* installer = CatalogueInstaller::newInstaller();
		installer->install();
		delete installer;		
	}
	
	void OptimizerCatalogue::uninstall() throw(MineruleException, odbc::SQLException) {
		CatalogueInstaller* installer = CatalogueInstaller::newInstaller();
		installer->uninstall();
		delete installer;		
	}

	void OptimizerCatalogue::addMineruleResult(const OptimizerCatalogue::MineruleResultInfo& mri )
	throw(odbc::SQLException, MineruleException){

		odbc::Connection* connection = 
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::string galid = addMineruleAttributeList(mri.ga);
		std::string ralid = addMineruleAttributeList(mri.ra);
		std::string calid = addMineruleAttributeList(mri.ca);
		std::string qryid = getNewAutoincrementValue("mr_query");
    //std::cout<<"PIPPO"<<std::endl;
		std::auto_ptr<odbc::Statement> statement( connection->createStatement() );
    //std::cout<<"PLUTO"<<std::endl;
		statement->execute( std::string("INSERT INTO mr_query ") +
		"(query_id, query_text, query_name, tab_results_name, source_tab_name," +
		"gal,ral,cal) VALUES ("+
		qryid+","+
		SQLUtils::quote(mri.getText())+","+
		SQLUtils::quote(mri.tab_result)+","+
		SQLUtils::quote(mri.resultset)+","+
		SQLUtils::quote(mri.tab_source)+","+
		galid+","+
		ralid+","+
		calid+")");
	}
  
	std::string OptimizerCatalogue::getResultsetName(const std::string& queryname) 
	throw(odbc::SQLException, MineruleException){
		odbc::Connection* connection =
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(
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
  
	std::string OptimizerCatalogue::getMRQueryName(size_t i) throw(odbc::SQLException, MineruleException) {
		//  -- This implementation is much more efficient but does not ensure that the result retrieved
		//		 are in the same order of the ones returned by getMRQueryInfos
		//
		// std::string queryNumber = Converter(i-1).toString();
		// odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		// 
		// std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		// std::auto_ptr<odbc::ResultSet> rs(statement->executeQuery("SELECT query_name FROM mr_query LIMIT 1 OFFSET "+queryNumber));
		// 
		// if( rs->next() && !rs->isAfterLast() ) {
		// 	return rs->getString(1);
		// } else {
		// 	throw MineruleException(MR_ERROR_CATALOGUE_ERROR, "Query number "+Converter(i).toString()+" not found");
		// }  	
		//  -- NEW IMPLEMENTATION
		
		std::vector<CatalogueInfo> catInfoVec;
		getMRQueryInfos(catInfoVec);
		if(i-1<catInfoVec.size()) {
			return catInfoVec[i-1].resName;
		} else {
			throw MineruleException(MR_ERROR_CATALOGUE_ERROR, "Query number "+Converter(i).toString()+" not found");			
		}
		
	}

	void OptimizerCatalogue::getMRQueryNames(std::vector<std::string>& nameVec) throw(odbc::SQLException, MineruleException) {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(statement->executeQuery("SELECT query_name FROM mr_query"));

		while( rs->next() ) {
			nameVec.push_back(rs->getString(1));
		}
	}
  
	void OptimizerCatalogue::setMRQueryInfoFromResultSet( odbc::ResultSet* rs, CatalogueInfo& info, bool includeResultSize ) {
		Connection connection;
	  
		info.qryName = rs->getString(1);
		info.qryText = rs->getString(2);
		info.resName = rs->getString(3);
		connection.setOutTableName(info.resName);
	  
		info.resTables.push_back(connection.getTableName(Connection::RulesTable));
		info.resTables.push_back(connection.getTableName(Connection::BodiesTable));
		info.resTables.push_back(connection.getTableName(Connection::HeadsTable));

		if(includeResultSize) 
			info.updateQrySize();
	}

	void OptimizerCatalogue::getMRQueryInfos(std::vector<CatalogueInfo>& catInfoVec, bool includeResultSize) 
	throw(odbc::SQLException, MineruleException) {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(statement->executeQuery(
			"SELECT query_name,query_text, tab_results_name, source_tab_name "
				"FROM mr_query"));

		while( rs->next() ) {
			CatalogueInfo info;
			setMRQueryInfoFromResultSet(rs.get(), info, includeResultSize);

			catInfoVec.push_back(info);
		}
	}


	void 
	OptimizerCatalogue::getMRQueryInfo(const std::string& qryName, CatalogueInfo& info, bool includeResultSize) throw(odbc::SQLException, MineruleException) {
		odbc::Connection* odbc_connection =	MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> statement(odbc_connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(
			statement->executeQuery("SELECT query_name,query_text, "
				"  tab_results_name, source_tab_name "
					"FROM mr_query "
						"WHERE query_name='"+qryName+"'"));

		if( rs->next() ) {
			setMRQueryInfoFromResultSet(rs.get(), info, includeResultSize);
		} else {
			throw MineruleException(MR_ERROR_CATALOGUE_ERROR,
				"Cannot find the query named:" + qryName +" "
				"Either there is some problem with the optimizer catalogue "
				"or a wrong query name has been specified");
		}
	}

	void OptimizerCatalogue::getMRQueryResultIterator(const std::string& qryName, QueryResult::Iterator& it, double supp, double conf) throw(odbc::SQLException, MineruleException) {
		CatalogueInfo catInfo;
		getMRQueryInfo( qryName, catInfo );
    
		ParsedMinerule p;
		MineruleOptions::getSharedOptions().getLogStreamObj().disable();
		p.init(catInfo.qryText);
		MineruleOptions::getSharedOptions().getLogStreamObj().enable();
		if(supp<0)  supp=p.sup;
		if(conf<0)  conf=p.conf;

		it.init(catInfo.resName, supp, conf);
	}

	void
	CatalogueInfo::updateQrySize() throw (odbc::SQLException, MineruleException){
		ParsedMinerule p;
		MineruleOptions::getSharedOptions().getLogStreamObj().disable();
		p.init(qryText);
		MineruleOptions::getSharedOptions().getLogStreamObj().enable();
    
		std::string qry = 
			"SELECT count(*) "
				"FROM "+resName+" "+
		"WHERE supp>="+Converter(p.sup).toString()+
		" AND con>="+Converter(p.conf).toString();

		odbc::Connection* connection =
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		std::auto_ptr<odbc::Statement> statement(connection->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(statement->executeQuery( qry ));
				 
		MRDebug() << "updateQrySize, the query is:" << qry << std::endl;

		if(rs->next()) {
			resSize = rs->getInt(1);
		} else {
			throw MineruleException(MR_ERROR_CATALOGUE_ERROR,
				"CatalogueInfo::updateQrySize - cannot"
					" retrieve the size of the result set");
		}
	}

	bool
	OptimizerCatalogue::isIDAttribute(const std::string& table,const ParsedMinerule::AttrVector& itemCols,const std::string& attribute) const throw(MineruleException)  {
		std::set<std::string> itemColsSet;
		copy( itemCols.begin(), itemCols.end(), std::insert_iterator< std::set<std::string> >(itemColsSet,itemColsSet.begin()));
	
		if(itemColsSet.find(attribute)!=itemColsSet.end()) {
		// if I am checking over the attributes that composes the attrlist then 
		// the attribute is trivially item dependent.
			return true;
		}

		Catalogue::const_iterator cat_it = depFunCatalogue.find(table);
		if(cat_it==depFunCatalogue.end())
			return false;

		const CatalogueEntry& ce = cat_it->second;
		CatalogueEntry::const_iterator ce_it;
		for( ce_it=ce.begin(); ce_it!=ce.end(); ce_it++ ) {
      // if I can find the attribute in the rhs of the fd
	  // and the item cols includes the lhs of the fd 
			if(ce_it->second.first.find(attribute)!=ce_it->second.first.end() && 
				includes(itemColsSet.begin(), itemColsSet.end(), ce_it->first.begin(), ce_it->first.end()))
			  // then the attribute is item dependent
					return true;
		}
    
		return false;
	}
  

} // namespace




  
