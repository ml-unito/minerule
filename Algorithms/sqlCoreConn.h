/* sqlCoreConn.h*/

#ifndef SQLCORECONN_H
#define SQLCORECONN_H

#include <string.h>
#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>
#include<odbc++/databasemetadata.h>
#include<odbc++/preparedstatement.h>

#include "MRDatabase/sourcerow.h"
#include "MRDatabase/sourcerowdescriptor.h"

#include <iostream>
#include <stdio.h>

#include "Algorithms.h"

namespace minerule {

class sqlCoreConn  {
  
  class DBInserter {
   protected:
   sqlCoreConn& coreConn;
   public:
	DBInserter(sqlCoreConn& cc) : coreConn(cc) {};
   virtual void insert_DB(const HeadBodyType&, 
			  const HeadBodyType&,  
			  double support,  
			  double confidence,
			  bool saveBody = true) =0;
   virtual void insert_DB_HBelems(const HeadBodyType& elems, size_t counter) =0;
   virtual ~DBInserter() {};
   virtual void init() {};
   virtual void finalize() {};
  };

  class DirectDBInserter : public DBInserter {
   public:
	DirectDBInserter(sqlCoreConn& cc) : DBInserter(cc) {};
   virtual void insert_DB(const HeadBodyType&, 
			  const HeadBodyType&,  
			  double support,  
			  double confidence,
			  bool saveBody = true);
   virtual void insert_DB_HBelems(const HeadBodyType& elems, size_t counter);
   virtual ~DirectDBInserter() {};
  };

  class CachedDBInserter : public DBInserter {
   ofstream outR, outHB;
   string filename;
   public:
	CachedDBInserter(sqlCoreConn& cc) : DBInserter(cc) {};
   virtual void insert_DB(const HeadBodyType&, 
			  const HeadBodyType&,  
			  double support,  
			  double confidence,
			  bool saveBody = true);
   virtual void insert_DB_HBelems(const HeadBodyType& elems, size_t counter);
   virtual ~CachedDBInserter() {};
   virtual void init();
   virtual void finalize();
  };

  string outTableName;
  //  const AlgorithmsOptions* algoOptions;
  MinMaxPair bodyCard;
  MinMaxPair headCard;
  DBInserter * dbInserter;
public:
   // Costruttore
  sqlCoreConn() : bodyCard(1,1000), headCard(1,1000)  /*algoOptions(NULL)*/ {
	if (MineruleOptions::getSharedOptions().getOdbc_db().getCacheWrites()) 
		dbInserter = new CachedDBInserter(*this);
	else dbInserter = new DirectDBInserter(*this);
  }
  // Distruttore
  ~sqlCoreConn() { delete dbInserter; }

  void setBodyCardinalities( const MinMaxPair& rhs) { bodyCard=rhs; }
  void setHeadCardinalities( const MinMaxPair& rhs) { headCard=rhs; }
   //   bool connect();
   void useConnection(odbc::Connection* newConnection);
   //   void disconnect();
   //   bool isConnected();
   odbc::Connection* getConnection() {return connection;}
   //odbc::ResultSet* openQuery();
   odbc::ResultSet* openQuery(char* Qry);
    odbc::ResultSet* openQuery(const char* Qry);
   //int getNumCols(odbc::ResultSet* result);
   //int getNumRows();
   bool existTable(const char * tableName);
   void deleteTable(const char * tableName);
   void deleteDestTable();
   void create_db_rule(int sintax);
   void insert_DB(const char * what);

   // this function should be systematically used in order to
   // write acquired rules to the DB
   // It uses algorithmOptions.get????Cardinalities() to filter rules
   // having wrong cardinalities
   void insert_DB(const HeadBodyType& body, 
		  const HeadBodyType& head,  
		  double support,  
		  double confidence,
		  bool saveBody = true) {
	dbInserter->insert_DB(body, head, support, confidence, saveBody);
    }

   /*   void setAlgorithmOptions(const AlgorithmsOptions& opts) {
     algoOptions = &opts;
     }*/

   //   odbc::ResultSet* getPartition(int begin,int end);

   void delete_tmp_db();
   void create_tmp_db(int sintax, 
		      const SourceRowAttrCollectionDescriptor& body, 
		      const SourceRowAttrCollectionDescriptor& head);

   void setOutTableName(const string& str) {
     outTableName=str;
   }

   void init() {
     dbInserter->init();
   }

   void finalize() { dbInserter->finalize(); }

   // name of the table where rules will be written
   const string& getOutTableName() const {
     return outTableName;
   }

   // when rules are written on the DB, the sqlCoreConn uses the 
   // following format: idBody, idHead, supp, conf, cardBody, cardHead
   // idBody and idHead are foreign (non unique) keys of the table whose
   // name can be retrieved using the following procedure.

   string getElemsOutTableName() const {
     return outTableName + "_elems";
   }
  // void closeQuery();
private:
   odbc::Connection* connection;

//   void insert_DB_HBelems(const HeadBodyType& elems, size_t counter);

};

// Costruttore

} // end namespace

#endif

