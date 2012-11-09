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

void Connection::deleteDestTable() {
  deleteTable(getOutTableName().c_str());
  deleteTable(getHeadsTableName().c_str());
  deleteTable(getBodiesTableName().c_str());
}

void Connection::deleteTable(const char * tableName)
{
   odbc::Statement* stmt=connection->createStatement();
   if (tableExists(tableName))
   {
 	std::string Qry;
	Qry = "DROP TABLE ";
	Qry += tableName;
	try {
	   stmt->execute(Qry);
	} catch (odbc::SQLException& e) { std::cerr << "Wrong statement " << Qry << std::endl; }
   }
   delete stmt;
}

// Bisogna controllare che non esista già la tabella
// altrimenti va in errore !!
void Connection::createResultTables(const SourceRowDescriptor& srd)
 {
    odbc::Statement* statement=connection->createStatement();
    std::string create, create_index;

	// Creating the rules table
    create=string("CREATE TABLE ")+getOutTableName()+" (bodyId int, headId int, supp float, con float, cardBody int, cardHead int );";	
    statement->execute(create);	
	
	// Creating the body elements table
    create=string("CREATE TABLE ")+ getBodiesTableName() + " (id int, elem varchar(255));";
	create_index = " CREATE INDEX "+getBodiesTableName()+"_index ON " + getBodiesTableName() + " (id);";

    statement->execute(create);
	statement->execute(create_index);

	// Creating the head elements table
    create=string("CREATE TABLE ")+ getHeadsTableName() + " (id int, elem varchar(255));";
	create_index = " CREATE INDEX "+getHeadsTableName()+"_index ON " + getHeadsTableName() + " (id);";

    statement->execute(create);
	statement->execute(create_index);

    delete statement;
 }


void Connection::DirectDBInserter::insertHeadBodyElems(const HeadBodyType& elems, size_t counter) {
//  assert( !elems.empty() );

  std::string query = 
    "INSERT INTO " + connection.getElemsOutTableName() + " VALUES (?,?)";
  odbc::PreparedStatement* state = connection.connection->prepareStatement(query);


  HeadBodyType::const_iterator it = elems.begin();
  for(; it!=elems.end() ; it++ ) {
    std::string str;
    SourceRowElement::serializeElementToString(it->getElement(), str);
    state->setLong(1,counter);
    state->setString(2,str);
    state->execute();
  }

  delete state;
}


void Connection::CachedDBInserter::insertHeadBodyElems(const HeadBodyType& elems, size_t counter) {
//  assert( !elems.empty() );

  HeadBodyType::const_iterator it = elems.begin();
  std::string str;
  for(; it!=elems.end() ; it++ ) {
    SourceRowElement::serializeElementToString(it->getElement(), str);
    outHB << counter << "\t" << str << std::endl;
  }

}

void Connection::CachedDBInserter::init() {
  filename = tmpnam(NULL);
  std::string temp = filename + ".r";
  outR.open(temp.c_str());
  temp = filename + ".hb";
  outHB.open(temp.c_str());
}

void Connection::CachedDBInserter::finalize() {
  outR.close();
  outHB.close();
  std::string loadstr1 = filename + ".r";
  chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  std::string loadstr2 = filename + ".hb";
  chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  loadstr1 = "LOAD DATA INFILE '";
  loadstr1 += filename;
  loadstr1 += ".r' INTO TABLE ";
  loadstr1 += connection.getOutTableName();
  loadstr2 = "LOAD DATA INFILE '";
  loadstr2 += filename;
  loadstr2 += ".hb' INTO TABLE ";
  loadstr2 += connection.getElemsOutTableName();
  odbc::Statement* state = connection.getODBCConnection()->createStatement();
  state->execute(loadstr1.c_str());
  state->execute(loadstr2.c_str());
  delete state;
  loadstr1 = filename + ".r";
  unlink(loadstr1.c_str());
  loadstr2 = filename + ".hb";
  unlink(loadstr2.c_str());
}

void Connection::CachedDBInserter::insert(const HeadBodyType& body,
			    const HeadBodyType& head,
			    double support,
			    double confidence,
			    bool saveBody) {

  // if either body or head elements are too many or too few with respect to
  // the allowed cardinalities, we simply return back to the caller.
  if( !connection.bodyCard.validate(body.size()) || !connection.headCard.validate(head.size()))
    return;

  static size_t counter=0;

  static size_t bodyId = counter;
  if (saveBody) {
	bodyId = counter++;
	insertHeadBodyElems(body, bodyId);
  }
  
  size_t headId = counter++;
  insertHeadBodyElems(head, headId);

  outR << bodyId << "\t" << headId << "\t" << support << "\t" << confidence << "\t" << body.size() << "\t" << head.size() << std::endl;
}

void Connection::DirectDBInserter::insert(const HeadBodyType& body,
			    const HeadBodyType& head,
			    double support,
			    double confidence,
                bool saveBody) {
  // if either body or head, is too big, too low with respect to
  // the allowed cardinalities we simply return back to the caller.
  if( !connection.bodyCard.validate(body.size()) || !connection.headCard.validate(head.size()))
    return;

  static size_t counter=0;

  static odbc::Statement* state = connection.connection->createStatement();

  static size_t bodyId = counter;
  if (saveBody) {
	bodyId = counter++;
	insertHeadBodyElems(body, bodyId);
  }
  
  size_t headId = counter++;
  insertHeadBodyElems(head, headId);

  stringstream query;
  query << "INSERT INTO " << connection.getOutTableName() 
		<< " VALUES (" << bodyId << "," << headId << "," 
		<< support << "," << confidence << "," << body.size() << "," << head.size() << ")" ;

  state->execute(query.str());
  //delete state;
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
    odbc::ResultSet* result;
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

    if( statement->execute(create) ) {
      //cout << "Result set available..." << std::endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }

    if (sintax==1)
    {
      create="CREATE TABLE tmp_Rule_Head_Ext(id int AUTO_INCREMENT PRIMARY KEY,id_head char(13),level int,"
	+ head.getSQLDataDefinition()+");";
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
