/* Database/Connection.h*/

#ifndef CONNECTION_H_3H6SFDND
#define CONNECTION_H_3H6SFDND

#include <string.h>
#include <minerule/mrdb/Connection.hpp>
#include <minerule/mrdb/ResultSet.hpp>
#include <minerule/mrdb/Statement.hpp>
#include <minerule/mrdb/ResultSetMetaData.hpp>
#include <minerule/mrdb/DatabaseMetaData.hpp>
#include <minerule/mrdb/PreparedStatement.hpp>

#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/Database/SourceRow.hpp"
#include "minerule/Database/SourceRowMetaInfo.hpp"

#include <iostream>
#include <stdio.h>

// #include "minerule/Algorithms/Algorithms.h"

namespace minerule {

	class Connection  {
	public:
        typedef enum { RulesTable, HeadsTable, BodiesTable, SeqTable, SeqElTable, ElemTable } TableKind;

	private:
		class DBInserter; // forward declaration

		std::string outTableName;
		MinMaxPair bodyCard;
		MinMaxPair headCard;
		DBInserter * dbInserter;
		mrdb::Connection* connection;

		class DBInserter {
		protected:
			Connection& connection;
			mrdb::PreparedStatement* headInserter;
			mrdb::PreparedStatement* bodyInserter;
			mrdb::PreparedStatement* seqInserter;
      mrdb::PreparedStatement* seqElInserter;
		public:
			DBInserter(Connection& cc) : connection(cc), headInserter(NULL), bodyInserter(NULL), seqInserter(NULL), seqElInserter(NULL) {};
			virtual ~DBInserter() {
				if(headInserter) delete headInserter;
				if(bodyInserter) delete bodyInserter;
				if(seqInserter) delete seqInserter;
        if(seqElInserter) delete seqElInserter;
			}
			virtual void setHeadInserter(mrdb::PreparedStatement* inserter) { headInserter = inserter; }
			virtual void setBodyInserter(mrdb::PreparedStatement* inserter) { bodyInserter = inserter; }
			virtual void setSeqInserter(mrdb::PreparedStatement* inserter) { seqInserter = inserter; }
      virtual void setSeqElInserter(mrdb::PreparedStatement* inserter) { seqElInserter = inserter; }
			virtual void insert(const ItemSet&, const ItemSet&, double support,  double confidence, bool saveBody = true) =0;
			virtual void insert(int seqId, int seqEl, double support, int pos, bool saveBody = true) =0;
			virtual void insertHeadBodyElems(TableKind,const ItemSet& elems, size_t counter) =0;
			virtual void init() {};
			virtual void finalize() {};
			virtual void finalize(bool b) {};
		};

		class DirectDBInserter : public DBInserter {
		public:
			DirectDBInserter(Connection& cc) : DBInserter(cc) {};
			virtual void insert(const ItemSet&, const ItemSet&, double support, double confidence,	bool saveBody = true);
			virtual void insert(int seqId, int seqEl, double support, int pos, bool saveSeqId = true);
			virtual void insertHeadBodyElems(TableKind, const ItemSet& elems, size_t counter);
			virtual ~DirectDBInserter() {};
		};

		class CachedDBInserter : public DBInserter {
		private:
			std::ofstream outR, outH, outB;
			std::string filename;
		public:
			CachedDBInserter(Connection& cc) : DBInserter(cc) {};
			virtual void insert(const ItemSet&, const ItemSet&, double support, double confidence,	bool saveBody = true);
			virtual void insert( int seqId, int seqEl,double support, int pos, bool saveSeqId = true);
			virtual void insertHeadBodyElems(TableKind, const ItemSet& elems, size_t counter);
			virtual ~CachedDBInserter() {};
			virtual void init();
			virtual void finalize();
			virtual void finalize(bool seq);
		};

	public:
		// Costruttore
		Connection() : bodyCard(1,1000), headCard(1,1000)  /*algoOptions(NULL)*/ {
			if (MineruleOptions::getSharedOptions().getMRDB().getCacheWrites())
				dbInserter = new CachedDBInserter(*this);
			else dbInserter = new DirectDBInserter(*this);
		}
		// Distruttore
		~Connection() {	delete dbInserter; }

		void setBodyCardinalities( const MinMaxPair& rhs ) { bodyCard=rhs; }
		void setHeadCardinalities( const MinMaxPair& rhs ) { headCard=rhs; }
		void useMRDBConnection(mrdb::Connection* newConnection);

		mrdb::Connection* getMRDBConnection() { return connection; }

		bool tableExists(const char * tableName);
		void deleteTable(const char * tableName);
		void deleteDestTables();
		void createResultTables(const SourceRowMetaInfo&) ;
		void createResultTables();
		void insert(const char * what);

		// this function should be systematically used in order to
		// write acquired rules to the DB
		// It uses algorithmOptions.get????Cardinalities() to filter rules
		// having wrong cardinalities
		void insert(const ItemSet& body,  const ItemSet& head, double support, double confidence, bool saveBody = true) {
			dbInserter->insert(body, head, support, confidence, saveBody);
		}
		//overload for sequences
      void insert( int seqId, int seqEl,double support, int pos, bool saveSeqId) {
          dbInserter->insert(seqId, seqEl, support, pos, saveSeqId);
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
		void finalize(bool b) { dbInserter->finalize(b); }

		std::string getTableName(TableKind kind) const;
	};
} // end namespace

#endif /* end of include guard: CONNECTION_H_3H6SFDND */
