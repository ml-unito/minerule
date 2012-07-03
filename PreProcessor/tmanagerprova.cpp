#include<iostream>
#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include"translationmanagerstandardsql.h"


using namespace std;
using namespace minerule;

int main(int argc, char** argv) {
  string dataSource = "logpolimi";
  string userName = "root";
  string userPwd = "mysql";

  string tableName = "FreqPageErrorBadAuthentication";
  string col = "customer_key";
  string val = "2";

  try {
    odbc::Connection* connection;

    if( argc<2 || string(argv[1])==string("mysql") ) {
      cout << "Using MySQL" << endl;
      connection=
	odbc::DriverManager::getConnection(
		       dataSource, 
		       userName, 
		       userPwd);
    } else {
      cout << "Using PostgreSQL" << endl;
      connection=
	odbc::DriverManager::getConnection("MineRule-psg", "esposito", "");
    }


    //    connection->setTraceFile("/tmp/odbclog");
    // connection->setTrace(true);

    TranslationManagerStandardSQL tmanager(connection,false);
    /*
    pair<bool, string> ret= tmanager.alreadyTranslated(tableName);

    if( ret.first ) {
      cout << "Tmanager said " 
	   << tableName
	   << " is translated" << endl;
    } else {
      cout << "Tmanager said " 
	   << tableName
	   << " sales_facts is untranslated" << endl;
	   }*/

    TranslatedTable* table = tmanager.translateTable(tableName);
    exit(1);

    cout << "Original name:" << table->getOriginalName() << endl;
    cout << "Translated name:" << table->getTranslatedName() << endl;

    /*
    cout << "Translated Column name " 
	 << col 
	 << table->getTranslatedColumnName(col) << endl;

    cout << "translated value ("
	 << col 
	 << ":" 
	 << val << ")" 
	 << table->getTranslatedValue(col, val) << endl;

    cout << "untranslate value (" 
	 << col 
	 << ":" 
	 << val 
	 << ")"
	 << table->getOriginalValue(col, 
	      	    table->getTranslatedValue(col, val)) 
	 << endl;
    */

    cout << "Cleaning up..." << endl;
    delete connection;
  } catch (odbc::SQLException e) {
    cout << "Something weird occurred, the error msg is:" << endl;
    cout << e.getMessage() << endl;
  }
}
