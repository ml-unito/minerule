#The following are the tables which MUST be present in the DB in order to make the optimizer working. The description is given in sql, in this way this file can be used to actually create the catalogue by passing it throught the sql server.( for instance cat README_OPTIMIZER_CATALOGUE.txt | mysql -Dtest does work for mysql and creates the catalogue in the "test" database).


# PAST QUERIES CATALOGUE
DROP TABLE IF EXISTS mr_query;
CREATE TABLE mr_query (query_id  int(10) NOT NULL,
		       query_text  text NOT NULL,
		       query_name  varchar(128) NOT NULL,	
		       tab_results_name varchar(128) NOT NULL,
		       source_tab_name varchar(128) NOT NULL,
		       gal  int(10) NOT NULL,
		       ral  int(10) NOT NULL,
		       cal  int(10),
		       PRIMARY KEY(query_id), 
		       INDEX(tab_results_name),
		       INDEX(query_name),
		       INDEX(source_tab_name));

# PAST QUERIES CATALOGUE COLUMN INDEX
DROP TABLE IF EXISTS mr_att_lists;
CREATE TABLE mr_att_lists (att_list_id  int(10) NOT NULL,
			   col_name varchar(128) NOT NULL,
			   INDEX (att_list_id));

# EQUIVALENT QUERIES CATALOGUE
DROP TABLE IF EXISTS mr_eq_keys;
CREATE TABLE mr_eq_keys (tab_name  varchar(128) NOT NULL,
			 key_id  int(10) NOT NULL,
			 ref_key_id  int(10) NOT NULL,
			 order_type char(1),
			 PRIMARY KEY(tab_name,key_id,ref_key_id));

# EQUIVALENT QUERIES CATALOGUE COLUMN INDEX
DROP TABLE IF EXISTS mr_eq_keys_col;
CREATE TABLE mr_eq_keys_col (key_id  int(10) NOT NULL,
			     col_name  varchar(128) NOT NULL,
			     INDEX(key_id));


# FUNCTIONAL DEPENDENCIES CATALOGUE
DROP TABLE IF EXISTS mr_dep_fun;
CREATE TABLE mr_dep_fun (lhs_tab_name  varchar(128) NOT NULL,
			 lhs_att_list_id  int(10) NOT NULL,
			 rhs_tab_name  varchar(128) NOT NULL,
			 rhs_att_list_id  int(10) NOT NULL,
			 order_type char(1),
			 PRIMARY KEY(lhs_tab_name,lhs_att_list_id,rhs_tab_name, rhs_att_list_id));

# EQUIVALENT QUERIES CATALOGUE COLUMN INDEX
DROP TABLE IF EXISTS mr_dep_fun_col;
CREATE TABLE mr_dep_fun_col (col_id  int(10) NOT NULL,
			     col_name  varchar(128) NOT NULL,
			     INDEX(col_id));

			   
# TABLE CONTAINING AUTOINCREMENT INFO
DROP TABLE IF EXISTS mr_autoincrement;
CREATE TABLE mr_autoincrement (table_name varchar(128) NOT NULL,
			       current_id int(10) NOT NULL,
			       PRIMARY KEY (table_name));

# INCREMENT VALUES FOR MR_AUTOINCREMENT
INSERT INTO mr_autoincrement (table_name, current_id) VALUES 
   ('mr_att_lists',0),('mr_eq_keys',0),('mr_query',0);
