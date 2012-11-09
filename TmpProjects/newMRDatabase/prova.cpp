#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <odbc++/drivermanager.h>
#include <odbc++/statement.h>
#include <odbc++/connection.h>
#include <odbc++/resultset.h>


int main() {
  odbc::Connection* conn = 
      odbc::DriverManager::getODBCConnection("test",
					 "root",
					 "mysql");

  odbc::Statement* state = conn->createStatement();
  odbc::ResultSet* rs = state->executeQuery("SELECT gid,item FROM Sales");
  std::set<int> s;
  std::set<int> s2;

  for(int i=0; i<1000000; i++) 
    s.insert(*new int(i));
}

