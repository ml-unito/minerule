#include "PreProcessor/translationmanagerstandardsql.h"
#include "PreProcessor/translatedtablestandardsql.h"
#include <odbc++/preparedstatement.h>
#include <iostream>
#include <sstream>
#include <cassert>


namespace minerule {


// ******************************************************************
// Initialization of static members
// ******************************************************************

const string
TranslationManagerStandardSQL::TM_PREFIX = "TMANAGER";

const string 
TranslationManagerStandardSQL::TABLEDICTIONARY_NAME = 
        TM_PREFIX + "_TABLEDICTIONARY";

const string 
TranslationManagerStandardSQL::TABLEDICTIONARY_ORIGINAL_TABLE = 
        "ORIGINAL_TABLE";

const int 
TranslationManagerStandardSQL::TABLEDICTIONARY_ORIGINAL_TABLE_LEN = 100;


const string 
TranslationManagerStandardSQL::TABLEDICTIONARY_TRANSLATED_TABLE =
        "TRANSLATED_TABLE";

const int
TranslationManagerStandardSQL::TABLEDICTIONARY_TRANSLATED_TABLE_LEN = 100;




// ******************************************************************
// Constructor 
// ******************************************************************
TranslationManagerStandardSQL::TranslationManagerStandardSQL(
			 odbc::Connection* aConnection,
			 bool translateEverythingOption) {
  assert(aConnection!=NULL);
  connection = aConnection;
  translateEverything = translateEverythingOption;
}


// ******************************************************************
// Public methods
// ******************************************************************

// - translateTable
//
// it returns a new access point for table ``tableName'' translating the
// table if necessary.
TranslatedTable* 
TranslationManagerStandardSQL::translateTable(const string& tableName) 
      const throw(odbc::SQLException) {
   if( !alreadyTranslated(tableName).first ) {
      doTranslateTable(tableName);
      updateTManagerDictionary(tableName);
  }
    
   return  newAccessPointForTable(tableName);
}

TranslatedTable*
TranslationManagerStandardSQL::newAccessPointForTable(const string& tableName) 
  const throw(odbc::SQLException) {

  TranslatedTableStandardSQL* result = 
    new TranslatedTableStandardSQL(this, 
				   tableName,
				   getTranslationTableNameForTable(tableName),
				   translateEverything);

  return result;
}





// - alreadyTranslated
// See header comments for input arguments and return values explanations
// The method accomplish the following:
//  1) ensures that a table dictionary exists
//  2) access the table dictionary looking for tableName
//  3) returns the requested info accordingly to the result of the query

pair<bool,string>
TranslationManagerStandardSQL::alreadyTranslated(const string& tableName) const 
  throw(odbc::SQLException)  {

  pair<bool, string> answer;
  odbc::Statement* statement = connection->createStatement();
  assert( statement != NULL );

  ensureTableDictionaryExist();

  odbc::ResultSet* result = 
    statement->executeQuery((
   string("SELECT *") +
   " FROM "  + TranslationManagerStandardSQL::TABLEDICTIONARY_NAME +
   " WHERE " + TranslationManagerStandardSQL::TABLEDICTIONARY_ORIGINAL_TABLE +
       "=" + quote(tableName)).c_str());
  
  // the result set will access the data most probably with a
  // forward-only cursor, next() is, hence, the only method we can
  // call to check if the result set is empty.
  if(result->next()) {
    answer.first = true;
    answer.second = 
      result->getString(TABLEDICTIONARY_TRANSLATED_TABLE.c_str()).c_str();
  } else {
    answer.first = false;
  }
   		    
  // Trashing the trashable
  delete result;
  delete statement;
  

  return answer;
}


// ******************************************************************
// Private methods
// ******************************************************************

void
TranslationManagerStandardSQL::updateTManagerDictionary(
			   const string& table) const throw(odbc::SQLException) {
  string query;
  query = string("INSERT INTO ") + 
    TranslationManagerStandardSQL::TABLEDICTIONARY_NAME +
    " VALUES ("+ quote(table) + ","+ quote(getTranslationTableNameForTable(table))+ ")";

  odbc::Statement* statement=connection->createStatement();

  try {
    statement->execute(query.c_str());
  } catch (...) {
    delete statement;
    throw;
  }

  delete statement;
}


// - quote
//
// quote a given string accordingly to the syntax of the sql92

string 
TranslationManagerStandardSQL::quote(const string& str) const {
  return "'"+str+"'";
}

// - quoteAttribute
// returns a valid quoted attribute name. Since it seems there is
// no standard way in SQL for quoting attribute names, the function
// simply returns the attribute name. Note that this means that you
// cannot have column names containing spaces or other special characters.

string
TranslationManagerStandardSQL::quoteAttribute(const string& str) const {
  //return "`"+str+"`";
  return str;
}


// - doTranslateTable
//
// Create the translated version of table ``tableName''.
// preconditions: tableName has not been already translated
//   and no table named TRANSLATENAME(tableName) does exist in the
//   database
// posconditions: a new table named TRANSLATENAME(tableName) will be
//       create along with a number of tables mantaining the binding between
//       original and translated values.
void
TranslationManagerStandardSQL::doTranslateTable(const string& tableName) const throw(odbc::SQLException) {
  odbc::Statement* statement = connection->createStatement();
  
  // Accessing metadata information of table "tableName"
  string query = "SELECT * FROM " + tableName + " WHERE 1=0";
  odbc::ResultSet* rs =
      statement->executeQuery(query.c_str());
  odbc::ResultSetMetaData* rsmd = rs->getMetaData();

  int i;
  try {
    for(i=1; i<=rsmd->getColumnCount(); i++) {
      if( shouldTranslateColumnsOfType(rsmd->getColumnType(i)) ) {
	BBLOG("starging translation of column " << i << " of table " << tableName);
	doTranslateColumn( 
			  tableName, 
			  i,
			  *rsmd);
	BBLOG("successful translated column " << i << " of table " << tableName);
      }
    }
    
    BBLOG("finalizing translation of table " << tableName);
    finalizeTranslationOfTable(tableName, rsmd );
    BBLOG("finalization done!");
  } catch (...) {
    // Cleaning up partially done work
    for( int j=1; j<i; j++ ) {
      if( shouldTranslateColumnsOfType(rsmd->getColumnType(i))) {
	dropTranslationForColumn( tableName, j, *rsmd );
      }
    }

    delete rs;
    delete statement;

    throw;
  }

  delete rs;
  delete statement;
}


void 
TranslationManagerStandardSQL::buildLists(
	    const string& table,
	    odbc::ResultSetMetaData* rsmd,
	    string& newTable,
	    string& selectList,
	    string& tableList,
	    string& conditionList ) const throw (odbc::SQLException) {
  // Full Column Name
#define FCN(table,col) (table)+"."+quoteAttribute(col)
#define ADDCOMMA(list) if( (list)!="" ) list+=","
#define ADDAND(list) if( (list)!="" ) list+=" AND "

  int colCount = rsmd->getColumnCount();
  newTable = getTranslationTableNameForTable(table);
  tableList = table;

  for( int i=1; i<=colCount; i++ ) {
    string colName = rsmd->getColumnName(i).c_str();

    if( shouldTranslateColumnsOfType( rsmd->getColumnType(i) ) ) {
      ADDCOMMA(selectList);
      ADDCOMMA(tableList);
      ADDAND(conditionList);

      string tColName = 	
	FCN(getTranslationTableNameForColumn(table,colName),
	    getTranslatedColumnNameForColumn(table,colName));

      string oColName =
	FCN(getTranslationTableNameForColumn(table,colName),
	    getOriginalColumnNameForColumn(table,colName));

      selectList += 
	tColName + " AS " + getTranslatedColumnNameForColumn(table,colName);

      tableList +=
	getTranslationTableNameForColumn(table, colName);
      
      conditionList += FCN(table,colName) + "=" + oColName;
    } else {
      ADDCOMMA(selectList);
      selectList+=
	FCN(table,colName) + " AS " + colName;
    }
  }

#undef FCN
#undef ADDCOMMA
#undef ADDAND
}

void
TranslationManagerStandardSQL::finalizeTranslationOfTable(
			const string& table,
			odbc::ResultSetMetaData* rsmd
			) const throw (odbc::SQLException) {
  string newTableName, selectList, tableList, conditionList;
  buildLists(table, rsmd, newTableName, selectList, tableList, conditionList);


 
  // string query;
 
  // wxString seems to have bugs when they sprintf large chunks of 
  // data... must resort to standard c string.
  string query;
  
  query = "CREATE TABLE ";
  query+= newTableName;
  query+=" AS SELECT ";
  query+=selectList;
  query+=" FROM ";
  query+=tableList;
  query+=" WHERE ";
  query+=conditionList;

  //cout << query << endl;

  odbc::Statement* statement = connection->createStatement();
  try {
    statement->execute(query.c_str());
  } catch (...) {
    delete statement;
    throw;
  }

  delete statement;
}

/*
void
TranslationManagerStandardSQL::createTranslationTableForColumn(
		 const string& table,
		 int colPos,
		 odbc::ResultSetMetaData& rsmd) 
                     const throw (odbc::SQLException) {
  odbc::Statement* statement = connection->createStatement();
  string column = rsmd.getColumnName(colPos).c_str();
  string colTypeName = rsmd.getColumnTypeName(colPos).c_str();
  
  string newTableName = 
    getTranslationTableNameForColumn(table,column);
  string translatedValueColName =
    getTranslatedColumnNameForColumn(table,column);
  string originalValueColName =
    getOriginalColumnNameForColumn(table,column);

  int precision=rsmd.getPrecision(colPos);
  int scale=rsmd.getScale(colPos);

  // FIXME: Bisogna cambiare il modo in cui si calcola il tipo
  // della nuova colonna da creare. In particolare sembra che:
  //   1. postgres SQL ritorni tipi strani (ad es. int4) che poi non
  //      accetta al momento di creare una nuova tavola
  //   2. mysql non riempa correttamente i campi precision e
  //      displaySize per i varchar, percio' non si ha nessuna informazione 
  //      sulla lunghezza dei campi stringa

  //  cout << table << "*!*:"<<rsmd.getColumnTypeName(1) <<endl;
  //  cout << "@@@" << precision << endl;
  //  cout << "###" << scale << endl;
  //  cout << "!!!" << rsmd.getColumnDisplaySize(1) << endl;

  if(precision>0) {
    if(scale>0) 
      colTypeName.sprintf("%s(%d,%d)",colTypeName.c_str(), precision,scale);
    else
      colTypeName.sprintf("%s(%d)",colTypeName.c_str(), precision);
  }
   

  // Creating the translation table
  string query; 
  query.sprintf("CREATE TABLE %s (%s %s, %s %s)",
		newTableName.c_str(),
		translatedValueColName.c_str(), 
		getIntTypeName().c_str(),
		originalValueColName.c_str(), 
		colTypeName.c_str());

  statement->execute(query.c_str());

  // Trashing the trashable
  delete statement;
}
*/

void
TranslationManagerStandardSQL::createTranslationTableForColumn(
		 const string& table,
		 int colPos,
		 odbc::ResultSetMetaData& rsmd) 
                     const throw (odbc::SQLException) {
  odbc::Statement* statement = connection->createStatement();
  string column = rsmd.getColumnName(colPos).c_str();
  string colTypeName = rsmd.getColumnTypeName(colPos).c_str();
  
  string newTableName = 
    getTranslationTableNameForColumn(table,column);
  string translatedValueColName =
    getTranslatedColumnNameForColumn(table,column);
  string originalValueColName =
    getOriginalColumnNameForColumn(table,column);


   // Creating the translation table
  string query; 
  query="CREATE TABLE " + newTableName + 
    " AS SELECT 0 as " + translatedValueColName + "," + originalValueColName  + 
    " FROM "+table+" WHERE 1=0";

  string query2;
  query2= "ALTER TABLE "+newTableName+
    " ADD UNIQUE ("+translatedValueColName+"), ADD UNIQUE ("+originalValueColName+")";
		
  try {
    statement->execute(query.c_str());
    statement->execute(query2.c_str());
  } catch (...) {
    delete statement;
    throw;
  }

  // Trashing the trashable
  delete statement;
}

void
TranslationManagerStandardSQL::dropTranslationForColumn(
		     const string& tableName,
		     int colPos,
		     odbc::ResultSetMetaData& rsmd 
		     ) const throw (odbc::SQLException) {
  string newTableName = 
    getTranslationTableNameForColumn(tableName, 
				     rsmd.getColumnName(colPos).c_str());

  string query = string("DROP TABLE "+newTableName);
  odbc::Statement* statement = connection->createStatement();
  try {
    statement->execute(query.c_str());
  } catch (...) {
    delete statement;
    throw;
  }

  delete statement;
}


void
TranslationManagerStandardSQL::doTranslateColumn(
		     const string& tableName,
		     int colPos,
		     odbc::ResultSetMetaData& rsmd
		     ) const throw(odbc::SQLException) {
  createTranslationTableForColumn(tableName, colPos, rsmd);
  string colName = rsmd.getColumnName(colPos).c_str();

  string newTableName = 
    getTranslationTableNameForColumn(tableName, colName);


  // Gathering data that will pupulate the new translation table
  string query; 
  query="SELECT DISTINCT "+colName+" FROM "+tableName+" ORDER BY "+colName;

  // Adding coded values...
  odbc::Statement* statement = connection->createStatement();
  odbc::ResultSet* rs = statement->executeQuery(query.c_str());
  odbc::ResultSetMetaData* column_rsmd = rs->getMetaData();
  
  int codeNum=0;
  query = string("INSERT INTO ")+newTableName+" VALUES (?,?)";
  odbc::PreparedStatement* pstm = 
    connection->prepareStatement( query.c_str() );

  try {
    while( rs->next() ) {
      pstm->setInt(1, codeNum++);
      doSetResultSetParameter(pstm, rs, column_rsmd);
      
      int affectedRows=pstm->executeUpdate();
      assert( affectedRows>0 );
    }
  } catch (...) {
    // trying to "roll back" succesful operations
    query = "DROP TABLE " + newTableName;
    statement->execute(query.c_str());

    delete rs;
    delete statement;
    delete pstm;
    throw;
  }


  // thrashing the thrashable
  delete rs;
  delete statement;
  delete pstm;
}

void
TranslationManagerStandardSQL::doSetResultSetParameter(
		       odbc::PreparedStatement* pstm,
		       odbc::ResultSet* rs,
		       odbc::ResultSetMetaData* rsmd) const {
  switch( rsmd->getColumnType(1) ) {
  default:
    pstm->setString(2,rs->getString(1));
  }
}




bool
TranslationManagerStandardSQL::shouldTranslateColumnsOfType(
					     int colType) const {
  if( translateEverything )
    return true;

  switch( colType ) {
  case odbc::Types::INTEGER: return false;
  case odbc::Types::SMALLINT: return false;
  default: return true;
  }
} 

// Checks if the table dictionary exists. if so it does nothing,
// otherwise try to create it. It throws an SQLException when it fails
// in creating the table.

void 
TranslationManagerStandardSQL::ensureTableDictionaryExist() const throw(odbc::SQLException) {
  assert(connection!=NULL);

  if( tableDictionaryExists() ) 
    return;

  odbc::Statement* statement = connection->createStatement();
  assert( statement!=NULL );

  
  stringstream querystream;
  querystream << "CREATE TABLE "+TABLEDICTIONARY_NAME+ " ("<<
    TABLEDICTIONARY_ORIGINAL_TABLE << " CHAR(" <<
    TABLEDICTIONARY_ORIGINAL_TABLE_LEN << ") PRIMARY KEY, " <<
    TABLEDICTIONARY_TRANSLATED_TABLE << " CHAR(" <<
    TABLEDICTIONARY_TRANSLATED_TABLE_LEN << ") NOT NULL)"; 

  string query = querystream.str();
  
  try {
    statement->execute(query.c_str());
  } catch (...) {
    delete statement;
    throw;
  }
  
  delete statement;
}

// Returns true if TABLEDICTIONARY_NAME can be found in the current db.
// Throws a SQLException if something goes wrong.

bool 
TranslationManagerStandardSQL::tableDictionaryExists() 
  const throw(odbc::SQLException)  {
  assert(connection!=NULL);
  bool answer;
  odbc::Statement* statement = connection->createStatement();
 
  string query = string(" SELECT * ") + 
    " FROM " + TABLEDICTIONARY_NAME + 
    " WHERE 1=0";
  
  try {
    statement->execute(query.c_str());
    answer = true;
  } catch (odbc::SQLException e) {
    // cout << "****" << e.getMessage() << endl << endl;
    answer = false;
  }
  
  delete statement;
  
  return answer;
}

} // namespace minerule
