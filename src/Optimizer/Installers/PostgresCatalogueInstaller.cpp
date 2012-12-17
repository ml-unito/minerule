#include "minerule/Optimizer/Installers/PostgresCatalogueInstaller.h"
#include "odbc++/statement.h"

namespace minerule {
	void PostgresCatalogueInstaller::dropMRQuery() 			throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_query");		
	}
	
	void PostgresCatalogueInstaller::dropMRAttList() 		throw(MineruleException, odbc::SQLException) 	{
		_statement->execute("DROP TABLE IF EXISTS mr_att_lists");		
	}
	
	void PostgresCatalogueInstaller::dropMREqKeys() 		throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_eq_keys");		
	}
	
	void PostgresCatalogueInstaller::dropMREqKeysCol() 		throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_eq_keys_col");		
	}
	
	void PostgresCatalogueInstaller::dropMRDepFun() 		throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_dep_fun");		
	}
	
	void PostgresCatalogueInstaller::dropMRDepFunCol() 		throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_dep_fun_col");		
	}
	
	void PostgresCatalogueInstaller::dropMRAutoincrement() 	throw(MineruleException, odbc::SQLException) {
		_statement->execute("DROP TABLE IF EXISTS mr_autoincrement");		
	}
	
	

	void PostgresCatalogueInstaller::installMRQuery() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_query (query_id  integer NOT NULL PRIMARY KEY,"
				       			"query_text  text NOT NULL,"
				       			"query_name  varchar(128) NOT NULL,	"
				       			"tab_results_name varchar(128) NOT NULL,"
				       			"source_tab_name varchar(128) NOT NULL,"
				       			"gal  integer NOT NULL,"
				       			"ral  integer NOT NULL,"
				   	   			"cal  integer)");
		_statement->execute("CREATE INDEX mr_query_tab_results_name_index ON mr_query(tab_results_name)");
		_statement->execute("CREATE INDEX mr_query_query_name_index ON mr_query (query_name)");
		_statement->execute("CREATE INDEX mr_query_source_tab_name_index ON mr_query (source_tab_name)");		
	}
	

	
	void PostgresCatalogueInstaller::installMRAttList() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_att_lists (att_list_id  integer NOT NULL,col_name varchar(128) NOT NULL)");			   
		_statement->execute("CREATE INDEX mr_att_lists_att_list_id_index ON mr_att_lists(att_list_id)");		
	}
	
	
	
	void PostgresCatalogueInstaller::installMREqKeys() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_eq_keys (tab_name  varchar(128) NOT NULL,"
					 			"key_id  integer NOT NULL,"
					 			"ref_key_id  integer NOT NULL,"
					 			"order_type char(1),"
					 			"CONSTRAINT mr_eq_keys_primary PRIMARY KEY(tab_name,key_id,ref_key_id))");		
	}
	
	
	
	void PostgresCatalogueInstaller::installMREqKeysCol() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_eq_keys_col (key_id  integer NOT NULL,"
					     		"col_name  varchar(128) NOT NULL)");
		_statement->execute("CREATE INDEX mr_eq_keys_col_key_id_index ON mr_eq_keys_col(key_id)");		
	}
	
	
	
	void PostgresCatalogueInstaller::installMRDepFun() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_dep_fun (lhs_tab_name  varchar(128) NOT NULL,"
					 			"lhs_att_list_id  integer NOT NULL,"
					 			"rhs_tab_name  varchar(128) NOT NULL,"
					 			"rhs_att_list_id  integer NOT NULL,"
					 			"order_type char(1),"
					 			"CONSTRAINT mr_dep_fun_primary PRIMARY KEY(lhs_tab_name,lhs_att_list_id,rhs_tab_name, rhs_att_list_id))");		
	}
	
	
	
	void PostgresCatalogueInstaller::installMRDepFunCol() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_dep_fun_col (col_id  integer NOT NULL, col_name  varchar(128) NOT NULL)");
		_statement->execute("CREATE INDEX mr_dep_fun_col_col_id_index ON mr_dep_fun_col(col_id)");		
	}
	
	
	
	void PostgresCatalogueInstaller::installMRAutoincrement() throw(MineruleException, odbc::SQLException) {
		_statement->execute("CREATE TABLE mr_autoincrement (table_name varchar(128) NOT NULL,"
					       		"current_id integer NOT NULL,"
					       		"CONSTRAINT mr_autoincrement_primary PRIMARY KEY (table_name))");

	}
	
	
	
	void PostgresCatalogueInstaller::initializeAutoincrement() throw(MineruleException, odbc::SQLException) {
		_statement->execute("INSERT INTO mr_autoincrement (table_name, current_id) VALUES ('mr_att_lists',0),('mr_eq_keys',0),('mr_query',0)");		
	}
	
	
	


}