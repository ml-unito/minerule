#include "sqlCoreConn.h"

#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
bool sqlCoreConn::connect()
 {
  bool connOK;
  cout<<"Connect..."<<endl;

  connOK=false;
  connection = odbc::DriverManager::getConnection("test", "root", "mysql");
  if (connection!= NULL)
      connOK=true;

  return connOK;
 }
*/

void
sqlCoreConn::useConnection(odbc::Connection* newConnection) {
  connection = newConnection;
}
/*
bool sqlCoreConn::isConnected()
 {
  bool connOK;

  connOK=false;
  if (connection!= NULL)
      connOK=true;

  return connOK;
 }
*/
/*
void sqlCoreConn::disconnect()
 {
  cout<<"Disconnect"<<endl;
  delete connection;
  odbc::DriverManager::shutdown();
 }
*/
/*
odbc::ResultSet* sqlCoreConn::openQuery()
 {
    odbc::Statement* statement=connection->createStatement();
    odbc::ResultSet* result;
    if( statement->execute("select tr,item from MRPROVA" ) ) {
      //cout << "Result set available..." << endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }

    return result;
 }
*/
odbc::ResultSet* sqlCoreConn::openQuery(const char* Qry)
 {
    odbc::Statement* statement=connection->createStatement();
    odbc::ResultSet* result;

    //cout<<"Sono in openQuery"<<endl;
    if( statement->execute(Qry) ) {
      // cout << "Result set available..." << endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }
  
    return result;

 }
odbc::ResultSet* sqlCoreConn::openQuery(char* Qry)
 {
    odbc::Statement* statement=connection->createStatement();
    odbc::ResultSet* result;

    if( statement->execute(Qry) ) {
      cout << "Result set available..." << endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }
    
    return result;

 }
/*
int sqlCoreConn::getNumCols(odbc::ResultSet* result)
 {
    odbc::ResultSetMetaData* metaData = result->getMetaData();
    int numCols = metaData->getColumnCount();

    return numCols;
 }

int sqlCoreConn::getNumRows()
 {
    odbc::Statement* statement=connection->createStatement();
    odbc::ResultSet* result;
    int numRows=0;

    if( statement->execute("select  distinct gidb from InputPartition;" ) )
    {
      cout << "Result set available..." << endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }
    if (result!=NULL)
    {
      //odbc::ResultSetMetaData* metaData = result->getMetaData();
      numRows = result->getRow();
    }
    //cout<<"sqlCoreconn.h NumRows "<<numRows<<endl;
    return numRows;
 }
*/

bool sqlCoreConn::existTable(const char * tableName)
{

	try {
		odbc::DatabaseMetaData* md;
		md=connection->getMetaData();
		odbc::ResultSet* rs;
		rs=md->getColumns("","",tableName,"");
	} catch (odbc::SQLException& e) { return false; }
	return true;
}

void sqlCoreConn::deleteDestTable() {
  deleteTable(getOutTableName().c_str());
  deleteTable(getElemsOutTableName().c_str());
}

void sqlCoreConn::deleteTable(const char * tableName)
{
   odbc::Statement* stmt=connection->createStatement();
   if (existTable(tableName))
   {
 	std::string Qry;
	Qry = "DROP TABLE ";
	Qry += tableName;
	try {
	   stmt->execute(Qry);
	} catch (odbc::SQLException& e) { cerr << "Wrong statement " << Qry << endl; }
   }
   delete stmt;
}

// Bisogna controllare che non esista già la tabella
// altrimenti va in errore !!
void sqlCoreConn::create_db_rule(int sintax)
 {
    odbc::Statement* statement=connection->createStatement();
    string create;

    // nb sintax is a deprecated parameter...

    create=string("CREATE TABLE ")+getOutTableName()+
	    " (bodyId int, headId int, supp float, con float, cardBody int, cardHead int );";
	
    statement->execute(create);

    create=string("CREATE TABLE ")+ getElemsOutTableName() +
	    " (id int, elem varchar(255), INDEX (id))";

    statement->execute(create);
    delete statement;
 }


void sqlCoreConn::DirectDBInserter::insert_DB_HBelems(const HeadBodyType& elems, size_t counter) {
//  assert( !elems.empty() );

  string query = 
    "INSERT INTO " + coreConn.getElemsOutTableName() + " VALUES (?,?)";
  odbc::PreparedStatement* state = coreConn.connection->prepareStatement(query);


  HeadBodyType::const_iterator it = elems.begin();
  for(; it!=elems.end() ; it++ ) {
    string str;
    SourceRowElement::serializeElementToString(it->getElement(), str);
    state->setLong(1,counter);
    state->setString(2,str);
    state->execute();
  }

  delete state;
}

/*
void sqlCoreConn::insert_DB(const std::set<ItemType>& body,
			    const std::set<ItemType>& head,
			    double support,
			    double confidence,
			    bool checkCard=false) {
  if( checkCard &&
      !bodyCard.validate(body.size()) 
      || !headCard.validate(head.size()))
    return;

  static size_t counter=0;

  odbc::Statement* state = connection->createStatement();

  size_t bodyId = counter++;
  insert_DB_HBelems(body, bodyId);
  
  size_t headId = counter++;
  insert_DB_HBelems(head, headId);

  stringstream query;
  query << "INSERT INTO " << getOutTableName() 
	<< " VALUES (" << bodyId << "," << headId << "," 
	<< support << "," << confidence << "," << body.size() << "," << head.size() << ")" ;

  state->execute(query.str());
  delete state;
  } */

void sqlCoreConn::CachedDBInserter::insert_DB_HBelems(const HeadBodyType& elems, size_t counter) {
//  assert( !elems.empty() );

  HeadBodyType::const_iterator it = elems.begin();
  string str;
  for(; it!=elems.end() ; it++ ) {
    SourceRowElement::serializeElementToString(it->getElement(), str);
    outHB << counter << "\t" << str << endl;
  }

}

void sqlCoreConn::CachedDBInserter::init() {
  filename = tmpnam(NULL);
  string temp = filename + ".r";
  outR.open(temp.c_str());
  temp = filename + ".hb";
  outHB.open(temp.c_str());
}

void sqlCoreConn::CachedDBInserter::finalize() {
  outR.close();
  outHB.close();
  string loadstr1 = filename + ".r";
  chmod(loadstr1.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  string loadstr2 = filename + ".hb";
  chmod(loadstr2.c_str(),S_IRUSR|S_IRGRP|S_IROTH);
  loadstr1 = "LOAD DATA INFILE '";
  loadstr1 += filename;
  loadstr1 += ".r' INTO TABLE ";
  loadstr1 += coreConn.getOutTableName();
  loadstr2 = "LOAD DATA INFILE '";
  loadstr2 += filename;
  loadstr2 += ".hb' INTO TABLE ";
  loadstr2 += coreConn.getElemsOutTableName();
  odbc::Statement* state = coreConn.getConnection()->createStatement();
  state->execute(loadstr1.c_str());
  state->execute(loadstr2.c_str());
  delete state;
  loadstr1 = filename + ".r";
  unlink(loadstr1.c_str());
  loadstr2 = filename + ".hb";
  unlink(loadstr2.c_str());
}

void sqlCoreConn::CachedDBInserter::insert_DB(const HeadBodyType& body,
			    const HeadBodyType& head,
			    double support,
			    double confidence,
			    bool saveBody) {

  //  assert(algoOptions!=NULL);

  // if either body or head, is too big, too low with respect to
  // the allowed cardinalities we simply return back to the caller.
  if( !coreConn.bodyCard.validate(body.size()) 
      || !coreConn.headCard.validate(head.size()))
    return;

  static size_t counter=0;

  static size_t bodyId = counter;
  if (saveBody) {
	bodyId = counter++;
	insert_DB_HBelems(body, bodyId);
  }
  
  size_t headId = counter++;
  insert_DB_HBelems(head, headId);

  outR << bodyId << "\t" << headId << "\t" << support << "\t" << confidence << "\t" << body.size() << "\t" << head.size() << endl;

}

void sqlCoreConn::DirectDBInserter::insert_DB(const HeadBodyType& body,
			    const HeadBodyType& head,
			    double support,
			    double confidence,
                            bool saveBody) {
  //  assert(algoOptions!=NULL);

  // if either body or head, is too big, too low with respect to
  // the allowed cardinalities we simply return back to the caller.
  if( !coreConn.bodyCard.validate(body.size()) 
      || !coreConn.headCard.validate(head.size()))
    return;

  static size_t counter=0;

  static odbc::Statement* state = coreConn.connection->createStatement();

  static size_t bodyId = counter;
  if (saveBody) {
	bodyId = counter++;
	insert_DB_HBelems(body, bodyId);
  }
  
  size_t headId = counter++;
  insert_DB_HBelems(head, headId);

  stringstream query;
  query << "INSERT INTO " << coreConn.getOutTableName() 
	<< " VALUES (" << bodyId << "," << headId << "," 
	<< support << "," << confidence << "," << body.size() << "," << head.size() << ")" ;

  state->execute(query.str());
  //delete state;
}

void sqlCoreConn::insert_DB(const char * what)
 {
    static odbc::Statement* statement=connection->createStatement();
    //    odbc::ResultSet* result;

    //    cerr << "000" << what << endl;

    if( statement->execute(what)==0 ) {
      //cout << "insert eseguita" << endl;
      //result =statement->getResultSet();
    } else
    {
      cout<<"insert fallita!"<<endl;
    }
    //delete statement;
 }

/*
odbc::ResultSet* sqlCoreConn::getPartition(int begin,int end)
 {
    odbc::Statement* statement=connection->createStatement();
    odbc::ResultSet* result;
    //    int numRows=0;
    std::string Qry;
    char beg[16],en[16];

    sprintf(beg,"%d",begin);
    sprintf(en,"%d",end);

    Qry="select * from InputPartition where (gidb>";
    Qry+=beg;
    Qry+=") and ( gidb<=";
    Qry+=en;
    Qry+=");";

    //cout<<"query: "<<Qry<<endl;
    if( statement->execute(Qry) )
    {
      cout << "Result set available..." << endl;
      result =statement->getResultSet();
    } else
    {
      result =NULL;
    }

    return result;
 }
*/
void sqlCoreConn::create_tmp_db(int sintax, 
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
      //cout << "Result set available..." << endl;
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

void sqlCoreConn::delete_tmp_db()
 {

  deleteTable("tmp_Rule_Base");
  deleteTable("tmp_Rule_Ext");
  deleteTable("tmp_Rule_Head_Ext");

 }
