#include "Database/Connection.h"

#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void
Connection::useODBCConnection(odbc::Connection* newConnection) {
  connection = newConnection;
}

string Connection::getTableName(TableKind kind) const {
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
		default:
			throw MineruleException(MR_ERROR_INTERNAL, "Unknown TableKind -- this is a bug, please report it!");
	}
}



bool Connection::tableExists(const char * tableName)
{
	try {
		odbc::DatabaseMetaData* md;
		md=connection->getMetaData();
		odbc::ResultSet* rs;
		rs=md->getColumns("","",tableName,"");
	} catch (odbc::SQLException& e) { 
		return false; 
	}
	
	return true;
}

void Connection::deleteDestTables() {
  deleteTable(getTableName(RulesTable).c_str());
  deleteTable(getTableName(HeadsTable).c_str());
  deleteTable(getTableName(BodiesTable).c_str());
}

void Connection::deleteTable(const char * tableName)
{
   odbc::Statement* stmt=connection->createStatement();
   MRLog() << "Dropping table " << tableName << std::endl;
   stmt->execute(std::string("DROP TABLE IF EXISTS ")+tableName);
   delete stmt;
}

// Bisogna controllare che non esista già la tabella
// altrimenti va in errore !!
void Connection::createResultTables(const SourceRowDescriptor& srd) {
	if(MineruleOptions::getSharedOptions().getSafety().getOverwriteHomonymMinerules())
		deleteDestTables();
		
	odbc::Statement* statement=connection->createStatement();
	std::string create, create_index;

	// Creating the rules table
	create=string("CREATE TABLE ")+getTableName(RulesTable)+" (bodyId int, headId int, supp float, con float, cardBody int, cardHead int );";	
	statement->execute(create);	
	
	// Creating the body elements table
	create=string("CREATE TABLE ")+ getTableName(BodiesTable) + " (id int, " + srd.getBody().getSQLDataDefinition() +")";
	create_index = " CREATE INDEX "+getTableName(BodiesTable)+"_index ON " + getTableName(BodiesTable) + " (id);";

	statement->execute(create);
	statement->execute(create_index);

	// Creating the head elements table
	create=string("CREATE TABLE ")+ getTableName(HeadsTable) +  " (id int, " + srd.getHead().getSQLDataDefinition() +")";
	create_index = " CREATE INDEX "+getTableName(HeadsTable)+"_index ON " + getTableName(HeadsTable) + " (id);";

	statement->execute(create);
	statement->execute(create_index);

	std::string headInserterQuery = "INSERT INTO " + getTableName(HeadsTable) + " VALUES (?,"+ srd.getHead().questionMarks() +")";
	dbInserter->setHeadInserter(connection->prepareStatement(headInserterQuery));

	std::string bodyInserterQuery = "INSERT INTO " + getTableName(BodiesTable) + " VALUES (?," + srd.getBody().questionMarks() + ")";
	dbInserter->setBodyInserter(connection->prepareStatement(bodyInserterQuery));

	delete statement;
 }


void Connection::DirectDBInserter::insertHeadBodyElems(TableKind kind, const ItemSet& elems, size_t counter) {
  MRLogStartMeasuring("Head/Body Insertion time:");
	
  assert( !elems.empty() );
  odbc::PreparedStatement* state;
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
  chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  
  std::string loadstr2 = filename + ".h";
  chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  
  std::string loadstr3 = filename + ".b";
  chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  
  const std::string& dbms = MineruleOptions::getSharedOptions().getOdbc_db().getDBMS();
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
		  "Option for key odbc::dbms is not set properly, it is set to "+dbms+
		  ", but only 'mysql' or 'postgres' are supported." );
  }
  
  odbc::Statement* state = connection.getODBCConnection()->createStatement();
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

  static odbc::Statement* state = connection.connection->createStatement();

  static size_t bodyId = counter;
  if (saveBody) {
	bodyId = counter++;
	insertHeadBodyElems(BodiesTable, body, bodyId);
  }
  
  size_t headId = counter++;
  insertHeadBodyElems(HeadsTable, head, headId);

  stringstream query;
  query << "INSERT INTO " << connection.getTableName(RulesTable) 
		<< " VALUES (" << bodyId << "," << headId << "," 
		<< support << "," << confidence << "," << body.size() << "," << head.size() << ")" ;

  state->execute(query.str());
  
  // delete state;
  MRLogStopMeasuring("Rule Insertion");
}

void Connection::insert(const char * what)
 {
    static odbc::Statement* statement=connection->createStatement();

    if( statement->execute(what)!=0 ) {
      std::cout<<"insert fallita!"<<std::endl;
    }

    delete statement;
 }


void Connection::create_tmp_db(int sintax, 
				const SourceRowAttrCollectionDescriptor& body, 
				const SourceRowAttrCollectionDescriptor& head)
 {
    odbc::Statement* statement=connection->createStatement();
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
