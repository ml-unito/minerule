/* Database/Connection.h*/

#ifndef CONNECTION_H_3H6SFDND
#define CONNECTION_H_3H6SFDND

#include <string.h>
#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>
#include<odbc++/databasemetadata.h>
#include<odbc++/preparedstatement.h>

#include "Utils/MineruleOptions.h"
#include "Database/SourceRow.h"
#include "Database/SourceRowDescriptor.h"

#include <iostream>
#include <stdio.h>

#include "Algorithms/Algorithms.h"

namespace minerule {

	class Connection  {
	public:
		typedef enum {
			RulesTable,
			HeadsTable,
			BodiesTable
		} TableKind;
		
	private:
		class DBInserter; // forward declaration

		std::string outTableName;
		MinMaxPair bodyCard;
		MinMaxPair headCard;
		DBInserter * dbInserter;
		odbc::Connection* connection;
  
		class DBInserter {
		protected:
			Connection& connection;
			odbc::PreparedStatement* headInserter; 
			odbc::PreparedStatement* bodyInserter; 
		public:
			DBInserter(Connection& cc) : connection(cc), headInserter(NULL), bodyInserter(NULL) {};
			virtual ~DBInserter() { 
				if(headInserter) delete headInserter;
				if(bodyInserter) delete bodyInserter;
			}
			virtual void setHeadInserter(odbc::PreparedStatement* inserter) { headInserter = inserter; }
			virtual void setBodyInserter(odbc::PreparedStatement* inserter) { bodyInserter = inserter; }
			virtual void insert(const ItemSet&, const ItemSet&, double support,  double confidence, bool saveBody = true) =0;
			virtual void insertHeadBodyElems(TableKind,const ItemSet& elems, size_t counter) =0;
			virtual void init() {};
			virtual void finalize() {};
		};

		class DirectDBInserter : public DBInserter {
		public:
			DirectDBInserter(Connection& cc) : DBInserter(cc) {};
			virtual void insert(const ItemSet&, const ItemSet&, double support, double confidence,	bool saveBody = true);
			virtual void insertHeadBodyElems(TableKind, const ItemSet& elems, size_t counter);
			virtual ~DirectDBInserter() {};
		};

		class CachedDBInserter : public DBInserter {
		private:
			ofstream outR, outHB;
			std::string filename;
		public:
			CachedDBInserter(Connection& cc) : DBInserter(cc) {};
			virtual void insert(const ItemSet&, const ItemSet&, double support, double confidence,	bool saveBody = true);
			virtual void insertHeadBodyElems(TableKind, const ItemSet& elems, size_t counter);
			virtual ~CachedDBInserter() {};
			virtual void init();
			virtual void finalize();
		};

	public:
		// Costruttore
		Connection() : bodyCard(1,1000), headCard(1,1000)  /*algoOptions(NULL)*/ {
			if (MineruleOptions::getSharedOptions().getOdbc_db().getCacheWrites()) 
				dbInserter = new CachedDBInserter(*this);
			else dbInserter = new DirectDBInserter(*this);
		}
		// Distruttore
		~Connection() {	delete dbInserter; }

		void setBodyCardinalities( const MinMaxPair& rhs ) { bodyCard=rhs; }
		void setHeadCardinalities( const MinMaxPair& rhs ) { headCard=rhs; }
		void useODBCConnection(odbc::Connection* newConnection);

		odbc::Connection* getODBCConnection() { return connection; }

		bool tableExists(const char * tableName);
		void deleteTable(const char * tableName);
		void deleteDestTables();

		void createResultTables(const SourceRowDescriptor&);
		void insert(const char * what);

		// this function should be systematically used in order to
		// write acquired rules to the DB
		// It uses algorithmOptions.get????Cardinalities() to filter rules
		// having wrong cardinalities
		void insert(const ItemSet& body,  const ItemSet& head, double support, double confidence, bool saveBody = true) {
			dbInserter->insert(body, head, support, confidence, saveBody);
		}

		void delete_tmp_db();
		void create_tmp_db(int sintax, const SourceRowAttrCollectionDescriptor& body, const SourceRowAttrCollectionDescriptor& head);

		void setOutTableName(const std::string& str) {
			outTableName=str;
		}

		void init() {
			dbInserter->init();
		}

		void finalize() { dbInserter->finalize(); }
		
		std::string getTableName(TableKind kind) const;
	};
} // end namespace

#endif /* end of include guard: CONNECTION_H_3H6SFDND */
