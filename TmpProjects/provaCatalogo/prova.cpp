#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/resultset.h>
#include <odbc++/databasemetadata.h>
#include <odbc++/drivermanager.h>
#include <iostream>

using namespace std;
using namespace odbc;

int main() {
  try {
    cout << "1";
    Connection* connection = 
      DriverManager::getConnection("test","root","mysql");
    
    cout << "2";
    DatabaseMetaData* dbmd =
      connection->getMetaData();

    ResultSet* rsprova = dbmd->getTables("","","",vector<string>());
    while( rsprova->next() ) {
      cout << ":"<<rsprova->getString(1) << ":" 
	   << rsprova->getString(2) << ":"
	   << rsprova->getString(3) << ":"
	   << rsprova->getString(4) << ":"
	   << rsprova->getString(5) << ":"
	   << endl;
    }

    cout << "3";

    ResultSet* rs = dbmd->getColumns("",
				     "",
				     "prova",
				     "id");
    
    cout << "4";
    while( rs->next() ) {
      cout << endl;
      cout << "coltype:" << rs->getShort(5) << endl;
      cout << "chartype:" << Types::CHAR << endl;
    }
  } catch (odbc::SQLException& e) {
    cout << e.getMessage() << endl;
  }catch (exception& e) {
    cout << e.what() << endl;
  }

}
