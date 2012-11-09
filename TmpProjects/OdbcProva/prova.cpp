#include <odbc++/drivermanager.h>
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/resultset.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  try {
    odbc::Connection* conn = 
      odbc::DriverManager::getODBCConnection("test",
					    "root",
					    "mysql");

    odbc::Statement* state = conn->createStatement(
			    odbc::ResultSet::TYPE_SCROLL_INSENSITIVE,
			    odbc::ResultSet::CONCUR_READ_ONLY);

    odbc::ResultSet* rs = state->executeQuery("SELECT * FROM prova");
    rs->absolute(4);

    while( rs->next() ) {
      cout << rs->getString(1) << endl;
    }
  } catch (odbc::SQLException& e) {
    cout << e.getMessage() << endl;
    throw;
  }
}
