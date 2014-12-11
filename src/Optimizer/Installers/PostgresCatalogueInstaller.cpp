//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
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
#include "minerule/Optimizer/Installers/PostgresCatalogueInstaller.hpp"
#include "minerule/mrdb/Statement.hpp"

namespace minerule {
	void PostgresCatalogueInstaller::dropMRQuery() 			{
		_statement->execute("DROP TABLE IF EXISTS mr_query");
	}

	void PostgresCatalogueInstaller::dropMRAttList() 		 {
		_statement->execute("DROP TABLE IF EXISTS mr_att_lists");
	}

	void PostgresCatalogueInstaller::dropMREqKeys() 		{
		_statement->execute("DROP TABLE IF EXISTS mr_eq_keys");
	}

	void PostgresCatalogueInstaller::dropMREqKeysCol() 		{
		_statement->execute("DROP TABLE IF EXISTS mr_eq_keys_col");
	}

	void PostgresCatalogueInstaller::dropMRDepFun() 		{
		_statement->execute("DROP TABLE IF EXISTS mr_dep_fun");
	}

	void PostgresCatalogueInstaller::dropMRDepFunCol() 		{
		_statement->execute("DROP TABLE IF EXISTS mr_dep_fun_col");
	}

	void PostgresCatalogueInstaller::dropMRAutoincrement() 	{
		_statement->execute("DROP TABLE IF EXISTS mr_autoincrement");
	}



	void PostgresCatalogueInstaller::installMRQuery() {
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



	void PostgresCatalogueInstaller::installMRAttList() {
		_statement->execute("CREATE TABLE mr_att_lists (att_list_id  integer NOT NULL,col_name varchar(128) NOT NULL)");
		_statement->execute("CREATE INDEX mr_att_lists_att_list_id_index ON mr_att_lists(att_list_id)");
	}



	void PostgresCatalogueInstaller::installMREqKeys() {
		_statement->execute("CREATE TABLE mr_eq_keys (tab_name  varchar(128) NOT NULL,"
					 			"key_id  integer NOT NULL,"
					 			"ref_key_id  integer NOT NULL,"
					 			"order_type char(1),"
					 			"CONSTRAINT mr_eq_keys_primary PRIMARY KEY(tab_name,key_id,ref_key_id))");
	}



	void PostgresCatalogueInstaller::installMREqKeysCol() {
		_statement->execute("CREATE TABLE mr_eq_keys_col (key_id  integer NOT NULL,"
					     		"col_name  varchar(128) NOT NULL)");
		_statement->execute("CREATE INDEX mr_eq_keys_col_key_id_index ON mr_eq_keys_col(key_id)");
	}



	void PostgresCatalogueInstaller::installMRDepFun() {
		_statement->execute("CREATE TABLE mr_dep_fun (lhs_tab_name  varchar(128) NOT NULL,"
					 			"lhs_att_list_id  integer NOT NULL,"
					 			"rhs_tab_name  varchar(128) NOT NULL,"
					 			"rhs_att_list_id  integer NOT NULL,"
					 			"order_type char(1),"
					 			"CONSTRAINT mr_dep_fun_primary PRIMARY KEY(lhs_tab_name,lhs_att_list_id,rhs_tab_name, rhs_att_list_id))");
	}



	void PostgresCatalogueInstaller::installMRDepFunCol() {
		_statement->execute("CREATE TABLE mr_dep_fun_col (col_id  integer NOT NULL, col_name  varchar(128) NOT NULL)");
		_statement->execute("CREATE INDEX mr_dep_fun_col_col_id_index ON mr_dep_fun_col(col_id)");
	}



	void PostgresCatalogueInstaller::installMRAutoincrement() {
		_statement->execute("CREATE TABLE mr_autoincrement (table_name varchar(128) NOT NULL,"
					       		"current_id integer NOT NULL,"
					       		"CONSTRAINT mr_autoincrement_primary PRIMARY KEY (table_name))");

	}



	void PostgresCatalogueInstaller::initializeAutoincrement() {
		_statement->execute("INSERT INTO mr_autoincrement (table_name, current_id) VALUES ('mr_att_lists',0),('mr_eq_keys',0),('mr_query',0)");
	}





}
