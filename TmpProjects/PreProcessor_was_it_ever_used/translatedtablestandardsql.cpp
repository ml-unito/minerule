#include "PreProcessor/translatedtablestandardsql.h"
#include "PreProcessor/translationmanagerstandardsql.h"
#include <cassert>


namespace minerule {

TranslatedTableStandardSQL::~TranslatedTableStandardSQL() {
  hash_map<const char*,bool, hash<const char*>, Scmp>::const_iterator it;
  for(it=columnsToTranslate.begin(); it!=columnsToTranslate.end(); it++) {
    delete it->first;
  }
}


void
TranslatedTableStandardSQL::updateColumnsToTranslate() {
  if( translateEverything )
    return;

  string query;
  query = "SELECT * FROM " + getOriginalName() + " WHERE 1=0";

  odbc::Statement* statement = tmanager->connection->createStatement();
  odbc::ResultSet* rs = statement->executeQuery(query.c_str());
  odbc::ResultSetMetaData* rsmd = rs->getMetaData();

  for( int i=1; i<=rsmd->getColumnCount(); i++ ) {
    char* colName = new char[rsmd->getColumnName(i).length()+1];
    strcpy( colName, rsmd->getColumnName(i).c_str() );

    if( tmanager->shouldTranslateColumnsOfType( rsmd->getColumnType(i) ) ) 
      columnsToTranslate[colName]=true;
    else
      columnsToTranslate[colName]=false;
  }

  delete statement;
}

string 
TranslatedTableStandardSQL::getTranslatedName() 
                 const throw (odbc::SQLException) {
  return translatedTable;
};

string
TranslatedTableStandardSQL::getOriginalName()  
                 const throw (odbc::SQLException) {
  return originalTable;
};

string
TranslatedTableStandardSQL::getTranslatedColumnName( 
				const string& columnName 
				) const throw (odbc::SQLException) {
  ColumnDictionary::const_iterator it =
    columnsToTranslate.find(columnName.c_str());
  assert(it!=columnsToTranslate.end());
    
  if( translateEverything || it->second )
    return tmanager->getTranslatedColumnNameForColumn(originalTable,columnName);
  else
    return columnName;
}

string
TranslatedTableStandardSQL::getTranslatedValue(
					       const string& columnName,
					       const string& value
			    ) const throw (odbc::SQLException) {
  ColumnDictionary::const_iterator it =
    columnsToTranslate.find(columnName.c_str());
  assert(it!=columnsToTranslate.end());

  if( !translateEverything && !it->second )
    return value;

  odbc::Statement* statement = tmanager->connection->createStatement();

  string query;
  string result;

  query= "SELECT "+ getTranslatedColumnName(columnName) +
        " FROM "+
    tmanager->getTranslationTableNameForColumn(
					       originalTable,
					       columnName) +
        " WHERE "+tmanager->getOriginalColumnNameForColumn(
							originalTable,
							columnName)+"="+tmanager->quote(value);

  //  cout << query << endl;

  try {
    odbc::ResultSet* rs = statement->executeQuery(query.c_str());

    if(rs->next())
      result = rs->getString(1).c_str();
    else {
      string msg;
      msg=string("TranslatedTable::getTranslatedValue:") + 
		  "original value '"+value+"' not found" +
		  " for column:'"+columnName+"' of table:'"+originalTable+"'";

      throw odbc::SQLException(msg);
    }

  } catch (...) {
    delete statement;
    throw;
  }

  delete statement;
  return result;
}

string
TranslatedTableStandardSQL::getOriginalValue(
					       const string& columnName,
					       const string& value
					       ) const 
                           throw (odbc::SQLException) {
  ColumnDictionary::const_iterator it =
    columnsToTranslate.find(columnName.c_str());
  assert(it!=columnsToTranslate.end());

  if( !translateEverything && !it->second )
    return value;

  odbc::Statement* statement = tmanager->connection->createStatement();

  string query;
  string result;

  query="SELECT "+
    tmanager->getOriginalColumnNameForColumn(
					     originalTable,
					     columnName)+
    " FROM "+tmanager->getTranslationTableNameForColumn(
				        originalTable,
					columnName)+
    " WHERE "+tmanager->getTranslatedColumnNameForColumn(
					originalTable,
					columnName)+
              "="+tmanager->quote(value);

  try {
    odbc::ResultSet* rs = statement->executeQuery(query.c_str());

    if(rs->next()) 
      result = rs->getString(1).c_str();
    else {
      string msg;
      msg=string("TranslatedTable::getOriginalValue:")+
		   " translated value '"+value+"' not found "+
		   "for column:'"+columnName+"' of table:'"+originalTable+"'";
      throw odbc::SQLException(msg);
    }

  } catch (...) {
    delete statement;
    throw;
  }

  delete statement;
  return result;
}
 
} // namespace minerule
