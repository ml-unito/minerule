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
#include "minerule/Database/Connection.hpp"

#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace minerule {

	void
	Connection::useMRDBConnection(mrdb::Connection* newConnection) {
	  connection = newConnection;
	}

	std::string Connection::getTableName(TableKind kind) const {
		switch(kind) {
			case RulesTable:
				return outTableName;
				break;
			case HeadsTable:
				return outTableName + "_head_elems";
				break;
			case BodiesTable:
				return outTableName + "_body_elems";
				break;
			case SeqTable:
          return outTableName + "_seq_support";
          break;
      case SeqElTable:
          return outTableName + "_seq";
          break;
      case ElemTable:
          return outTableName + "_seq1";
          break;
			default:
				throw MineruleException(MR_ERROR_INTERNAL, "Unknown TableKind -- this is a bug, please report it!");
		}
	}



	bool Connection::tableExists(const char * tableName){
			std::string table(tableName, strlen(tableName));
			std::string query = "SELECT relname FROM pg_class WHERE relname = '"+table+"';";
			mrdb::Statement* stmt=connection->createStatement();
			mrdb::ResultSet* rs = stmt->executeQuery(query);
			bool result = rs->next();
			delete rs;
			delete stmt;
			return result;
	}

	void Connection::deleteDestTables() {
	  deleteTable(getTableName(RulesTable).c_str());
	  deleteTable(getTableName(HeadsTable).c_str());
	  deleteTable(getTableName(BodiesTable).c_str());
	}

	void Connection::deleteTable(const char * tableName)
	{
	   mrdb::Statement* stmt=connection->createStatement();
	   MRLog() << "Dropping table " << tableName << std::endl;
	   stmt->execute(std::string("DROP TABLE IF EXISTS ")+tableName);
	   delete stmt;
	}

	// Bisogna controllare che non esista già la tabella
	// altrimenti va in errore !!
	void Connection::createResultTables(const SourceRowMetaInfo& srd) {
		if(MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules())
			deleteDestTables();

		mrdb::Statement* statement=connection->createStatement();
		std::string create, create_index;

		// Creating the rules table
		if(!tableExists(getTableName(RulesTable).c_str())){
			create=std::string("CREATE TABLE ")+getTableName(RulesTable)+" (bodyId int, headId int, supp float, con float, cardBody int, cardHead int );";
			statement->execute(create);
		}

		// Creating the body elements table
		if(!tableExists(getTableName(BodiesTable).c_str())){
			create=std::string("CREATE TABLE ")+ getTableName(BodiesTable) + " (id int, " + srd.getBody().getSQLDataDefinition() +")";
			create_index = " CREATE INDEX "+getTableName(BodiesTable)+"_index ON " + getTableName(BodiesTable) + " (id);";

			statement->execute(create);
			statement->execute(create_index);
		}
		if(srd.getHead().getColumnsCount() > 0) {
				// Creating the head elements table
				if(!tableExists(getTableName(HeadsTable).c_str())){
					create=std::string("CREATE TABLE ")+ getTableName(HeadsTable) +  " (id int, " + srd.getHead().getSQLDataDefinition() +")";

					create_index = " CREATE INDEX "+getTableName(HeadsTable)+"_index ON " + getTableName(HeadsTable) + " (id);";

					statement->execute(create);
					statement->execute(create_index);
				}

				std::string headInserterQuery = "INSERT INTO " + getTableName(HeadsTable) + " VALUES (?,"+ srd.getHead().questionMarks() +")";
				dbInserter->setHeadInserter(connection->prepareStatement(headInserterQuery));
		}
		std::string bodyInserterQuery = "INSERT INTO " + getTableName(BodiesTable) + " VALUES (?," + srd.getBody().questionMarks() + ")";
		dbInserter->setBodyInserter(connection->prepareStatement(bodyInserterQuery));

		delete statement;
	 }

	//overload for sequences
	void Connection::createResultTables(){

		std::cout<<"Calling mine sequence dest table function creation"<<std::endl;

		if(tableExists(getTableName(SeqTable).c_str()) && MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules())
				deleteDestTables();

		mrdb::Statement* statement=connection->createStatement();
		std::string create, create_index;

		// Creating the seq table
		if(!tableExists(getTableName(SeqTable).c_str())){
			create=std::string("CREATE TABLE ")+getTableName(SeqTable)+" (ID int, supp float );";
			create_index = " CREATE INDEX "+getTableName(SeqTable)+"_index ON " + getTableName(SeqTable) + " (id);";
			statement->execute(create);
			statement->execute(create_index);
		}

		// Creating the sequences' elements table
		if(!tableExists(getTableName(SeqElTable).c_str())){
			create=std::string("CREATE TABLE ")+ getTableName(SeqElTable) + " (id int,idEl int,pos int )";
			create_index = " CREATE INDEX "+getTableName(SeqElTable)+"_index ON " + getTableName(SeqElTable) + " (id);";

			statement->execute(create);
			statement->execute(create_index);
		}

		std::string seqInserterQuery = "INSERT INTO " + getTableName(SeqTable) + " VALUES (?, ?)";
		dbInserter->setSeqInserter(connection->prepareStatement(seqInserterQuery));

		std::string seqElInserterQuery = "INSERT INTO " + getTableName(SeqElTable) + " VALUES (?, ?, ?)";
		dbInserter->setSeqElInserter(connection->prepareStatement(seqElInserterQuery));

		delete statement;
	}


	void Connection::DirectDBInserter::insertHeadBodyElems(TableKind kind, const ItemSet& elems, size_t counter) {
	  MRLogStartMeasuring("Head/Body Insertion time:");

	  assert( !elems.empty() );
	  mrdb::PreparedStatement* state;
	  switch(kind) {
		  case BodiesTable:
			  state = bodyInserter;
			  break;
		  case HeadsTable:
			  state = headInserter;
			  break;
		  default:
			  throw MineruleException(MR_ERROR_INTERNAL, "bad call to insertHeadBodyElems. This is a bug! Please report it!");
	  }

	  ItemSet::const_iterator it = elems.begin();
	  for(; it!=elems.end() ; it++ ) {
	    state->setLong(1,counter);
		it->setPreparedStatementParameters(state,2);
	    state->execute();
	  }

	  MRLogStopMeasuring("Head/Body Insertion time:");
	}


	void Connection::CachedDBInserter::insertHeadBodyElems(TableKind kind, const ItemSet& elems, size_t id) {
	//  assert( !elems.empty() );
	    MRLogStartMeasuring("Head/Body Insertion time:");

		std::ofstream* outStream = NULL;
		switch(kind) {
			case RulesTable:
				outStream = &outR;
				break;
			case HeadsTable:
				outStream = &outH;
				break;
			case BodiesTable:
				outStream = &outB;
				break;
			default:
				throw MineruleException( MR_ERROR_INTERNAL, "Unexpected TableKind value." );
		}

		ItemSet::const_iterator it = elems.begin();
		std::string str;
		for(; it!=elems.end() ; it++ ) {
			(*outStream) << id << "\t" << it->getElement().asString("\t") << std::endl;
		}

	    MRLogStopMeasuring("Head/Body Insertion time:");
	}

	void Connection::CachedDBInserter::init() {
	  MRLogStartMeasuring("CachedDBInserter init");

	  char tmpFName[30];
	  strcpy(tmpFName,"/tmp/cacheDBInserterXXXXXX");
	  if(mkstemp(tmpFName)==-1) {
		  throw MineruleException(MR_ERROR_OUTPUT_FILE_PROBLEM, std::string("Cannot create temporary file:")+tmpFName);
	  }

	  filename = tmpFName;

	  MRLog() << "CachedDBInserter will use filestem: " << filename << std::endl;

	  std::string temp = filename + ".r";
	  outR.open(temp.c_str());

	  temp = filename + ".h";
	  outH.open(temp.c_str());

	  temp = filename + ".b";
	  outB.open(temp.c_str());
	  MRLogStopMeasuring("CachedDBInserter init");
	}

	void Connection::CachedDBInserter::finalize() {
	  MRLogStartMeasuring("CachedDBInserter finalize");

	  outR.close();
	  outH.close();
	  outB.close();

	  std::string loadstr1 = filename + ".r";
	  if(chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH)==-1) {
		  throw MineruleException( MR_ERROR_INTERNAL,
			  std::string("Cannot change permissions on file ")+loadstr1 +
			  " reason is:" + strerror(errno));
	  }

	  std::string loadstr2 = filename + ".h";
	  if(chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH)==-1) {
		  throw MineruleException( MR_ERROR_INTERNAL,
			  std::string("Cannot change permissions on file ")+loadstr2 +
			  " reason is:" + strerror(errno));
	  }

	  std::string loadstr3 = filename + ".b";
	  if(chmod(loadstr3.c_str(),S_IRUSR|S_IRGRP|S_IROTH)==-1) {
		  throw MineruleException( MR_ERROR_INTERNAL,
			  std::string("Cannot change permissions on file ")+loadstr3 +
			  " reason is:" + strerror(errno));
	  }

	  const std::string& dbms = MineruleOptions::getSharedOptions().getMRDB().getDBMS();
	  if(  dbms == "mysql" ) {
		  loadstr1 = "LOAD DATA INFILE '" + filename + ".r' INTO TABLE " + connection.getTableName(RulesTable);
		  loadstr2 = "LOAD DATA INFILE '" + filename + ".h' INTO TABLE " + connection.getTableName(HeadsTable);
		  loadstr3 = "LOAD DATA INFILE '" + filename + ".b' INTO TABLE " + connection.getTableName(BodiesTable);
	  } else if( dbms == "postgres" ) {
		  loadstr1 = "COPY " + connection.getTableName(RulesTable)  +" FROM '" + filename + ".r'";
		  loadstr2 = "COPY " + connection.getTableName(HeadsTable)  +" FROM '" + filename + ".h'";
		  loadstr3 = "COPY " + connection.getTableName(BodiesTable) +" FROM '" + filename + ".b'";
	  } else  {
		  throw MineruleException( MR_ERROR_OPTION_CONFIGURATION,
			  "Option for key mrdb::dbms is not set properly, it is set to "+dbms+
			  ", but only 'mysql' or 'postgres' are supported." );
	  }

	  mrdb::Statement* state = connection.getMRDBConnection()->createStatement();
	  state->execute(loadstr1.c_str());
	  state->execute(loadstr2.c_str());
	  state->execute(loadstr3.c_str());

	  delete state;

	  loadstr1 = filename + ".r";
	  unlink(loadstr1.c_str());

	  loadstr2 = filename + ".h";
	  unlink(loadstr2.c_str());

	  loadstr2 = filename + ".b";
	  unlink(loadstr2.c_str());

	  MRLogStopMeasuring("CachedDBInserter finalize");
	}

	//overload for sequences
  void Connection::CachedDBInserter::finalize(bool seq) {
    MRLogStartMeasuring("CachedDBInserter finalize");
    if(seq) std::cout<<"Saving sequences on db..."<<std::endl;
    outH.close();
    outB.close();

    std::string loadstr2 = filename + ".h";
    if(chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH)==-1) {
        throw MineruleException( MR_ERROR_INTERNAL,
            std::string("Cannot change permissions on file ")+loadstr2 +
            " reason is:" + strerror(errno));
    }

    std::string loadstr3 = filename + ".b";
    if(chmod(loadstr3.c_str(),S_IRUSR|S_IRGRP|S_IROTH)==-1) {
        throw MineruleException( MR_ERROR_INTERNAL,
            std::string("Cannot change permissions on file ")+loadstr3 +
            " reason is:" + strerror(errno));
    }

    const std::string& dbms = MineruleOptions::getSharedOptions().getMRDB().getDBMS();
    if(  dbms == "mysql" ) {
        loadstr2 = "LOAD DATA INFILE '" + filename + ".h' INTO TABLE " + connection.getTableName(SeqTable);
        loadstr3 = "LOAD DATA INFILE '" + filename + ".b' INTO TABLE " + connection.getTableName(SeqElTable);
    } else if( dbms == "postgres" ) {
        loadstr2 = "COPY " + connection.getTableName(SeqTable)  +" FROM '" + filename + ".h'";
        loadstr3 = "COPY " + connection.getTableName(SeqElTable) +" FROM '" + filename + ".b'";
    } else  {
        throw MineruleException( MR_ERROR_OPTION_CONFIGURATION,
            "Option for key mrdb::dbms is not set properly, it is set to "+dbms+
            ", but only 'mysql' or 'postgres' are supported." );
    }

    mrdb::Statement* state = connection.getMRDBConnection()->createStatement();
    state->execute(loadstr2.c_str());
    state->execute(loadstr3.c_str());

    delete state;

    loadstr2 = filename + ".h";
    unlink(loadstr2.c_str());

    loadstr3 = filename + ".b";
    unlink(loadstr3.c_str());

    MRLogStopMeasuring("CachedDBInserter finalize");
  }

	void Connection::CachedDBInserter::insert(const ItemSet& body,
				    const ItemSet& head,
				    double support,
				    double confidence,
				    bool saveBody) {
	  // if either body or head elements are too many or too few with respect to
	  // the allowed cardinalities, we simply return back to the caller.
	  if( !connection.bodyCard.validate(body.size()) || !connection.headCard.validate(head.size()))
	    return;

	  MRLogStartMeasuring("Rule Insertion");

	  static size_t counter=0;

	  static size_t bodyId = counter;
	  if (saveBody) {
		bodyId = counter++;
		insertHeadBodyElems(BodiesTable, body, bodyId);
	  }

	  size_t headId = counter++;
	  insertHeadBodyElems(HeadsTable, head, headId);

	  outR << bodyId << "\t" << headId << "\t" << support << "\t" << confidence << "\t" << body.size() << "\t" << head.size() << std::endl;

	  MRLogStopMeasuring("Rule Insertion");
	}

	void Connection::DirectDBInserter::insert(const ItemSet& body,
				    const ItemSet& head,
				    double support,
				    double confidence,
	                bool saveBody) {

	  // if either body or head, is too big, too low with respect to
	  // the allowed cardinalities we simply return back to the caller.
	  if( !connection.bodyCard.validate(body.size()) || !connection.headCard.validate(head.size()))
	    return;

	  MRLogStartMeasuring("Rule Insertion");

	  static size_t counter=0;

	  static mrdb::Statement* state = connection.connection->createStatement();

	  static size_t bodyId = counter;
	  if (saveBody) {
		bodyId = counter++;
		insertHeadBodyElems(BodiesTable, body, bodyId);
	  }

	  size_t headId = counter++;
	  insertHeadBodyElems(HeadsTable, head, headId);

	  std::stringstream query;
	  query << "INSERT INTO " << connection.getTableName(RulesTable)
			<< " VALUES (" << bodyId << "," << headId << ","
			<< support << "," << confidence << "," << body.size() << "," << head.size() << ")" ;

	  state->execute(query.str());

	  // delete state;
	  MRLogStopMeasuring("Rule Insertion");
	}

	void Connection::DirectDBInserter::insert(int seqId, int seqEl, double support, int pos, bool saveSeqId) {

    MRLogStartMeasuring("Sequences Insertion time:");

    mrdb::PreparedStatement* state;
    if(saveSeqId) {
      state = seqInserter;
      state->setInt(1,seqId);
      state->setDouble(2,support);
      state->execute();
    }
    state = seqElInserter;
    state->setInt(1,seqId);
    state->setInt(2,seqEl);
    state->setInt(3,pos);
    state->execute();

    MRLogStopMeasuring("Sequences Insertion time:");

  }

  void Connection::CachedDBInserter::insert(int seqId, int seqEl, double support, int pos, bool saveSeqId) {
    MRLogStartMeasuring("Sequences Insertion time:");

    std::ofstream* outStream = NULL;
    if(saveSeqId) {
      outStream = &outH;
      (*outStream) << seqId << "\t"<< support << std::endl;
    }
    outStream = &outB;

    (*outStream) << seqId << "\t" << seqEl << "\t" << pos << std::endl;

    MRLogStopMeasuring("Sequences Insertion time:");
  }

	void Connection::insert(const char * what){
	    static mrdb::Statement* statement=connection->createStatement();
	    statement->execute(what);
	    delete statement;
	 }


	void Connection::create_tmp_db(int sintax,
					const SourceRowAttrCollectionDescriptor& body,
					const SourceRowAttrCollectionDescriptor& head)
	 {
	    mrdb::Statement* statement=connection->createStatement();
	    std::string create;

	    delete_tmp_db();

	    switch (sintax)
	    {
		case 0 : create="CREATE TABLE tmp_Rule_Base(id int AUTO_INCREMENT PRIMARY KEY,level int, "
			   + body.getSQLDataDefinition() +");";
		         break;
		case 1 : create=
			   "CREATE TABLE tmp_Rule_Ext(id int AUTO_INCREMENT PRIMARY KEY,level int,"
			   + body.getSQLDataDefinition() + ", id_head char(13));";
		         break;
	    }

	    statement->execute(create);

	    if (sintax==1)
	    {
	      create="CREATE TABLE tmp_Rule_Head_Ext(id int AUTO_INCREMENT PRIMARY KEY,id_head char(13),level int," + head.getSQLDataDefinition()+");";
	      statement->execute(create);
	    }
	    delete statement;
	 }

	void Connection::delete_tmp_db()
	 {

	  deleteTable("tmp_Rule_Base");
	  deleteTable("tmp_Rule_Ext");
	  deleteTable("tmp_Rule_Head_Ext");

	 }

}
