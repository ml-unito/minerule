#include <iostream>
#include <memory>
#include <assert.h>
#include <dlfcn.h>

#include <libpq-fe.h>

#include <minerule/Utils/StringUtils.hpp>
#include <minerule/Utils/Converter.hpp>

#include "../Connection.hpp"
#include "../ResultSet.hpp"
#include "../Statement.hpp"
#include "../PreparedStatement.hpp"

#include <memory>

// GLOBAL CONNECTION FUNCTION
namespace mrdb_test {
  MRDBConnectFun connect;
}

void buildFixtures() {
  PGconn* connection = PQconnectdb("host=localhost user=pgtest");

  if(PQstatus(connection) != CONNECTION_OK) {
    std::cerr << "Error connecting to the database:" << PQerrorMessage(connection) << std::endl;
    exit(1);
  }


  PGresult* result;

  result = PQexec(connection, "DROP TABLE IF EXISTS mrdb_qry_test");
  result = PQexec(connection, "CREATE TABLE mrdb_qry_test (id int, str varchar, flt float)");
  if(PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error creating fixture table" << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  result = PQexec(connection,"INSERT INTO mrdb_qry_test (id,str,flt) VALUES (1, 'abc', 4.5), (2,'def', 5.4)");
  if(PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error populating fixture table" << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  PQfinish(connection);
}

void cleanupFixtures() {
  PGconn* connection = PQconnectdb("host=localhost user=pgtest");

  if(PQstatus(connection) != CONNECTION_OK) {
    std::cerr << "Error connecting to the database:" << PQerrorMessage(connection) << std::endl;
    exit(1);
  }

  PGresult* result = PQexec(connection, "DROP TABLE mrdb_qry_test;");
  if(PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error dropping fixture table" << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  PQfinish(connection);
}

void OK(const std::string& msg) {
  std::cout << "[" << minerule::StringUtils::toGreen("OK") <<  "] " << msg << std::endl;
}

void FAIL(const std::string& msg) {
  std::cout << "[" << minerule::StringUtils::toRed("ERROR") <<  "] " << msg << std::endl;
  exit(1);
}



MRDBConnectFun load_mrdb_pg_dylib() {
  void *handle = dlopen("libmrdb_pg.so", RTLD_LAZY);

  if (handle==NULL) {
    std::cout << "Error:" << dlerror() << std::endl;
    exit(1);
  }

  MRDBConnectFun connect = (MRDBConnectFun) dlsym(handle, "connect");

  if (connect==NULL) {
    std::cout << "Error:" << dlerror() << std::endl;
    exit(1);
  }

  return connect;
}


void testCanRetrieveTableNames() {
  buildFixtures();
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  mrdb::DatabaseMetaData* md = conn->getMetaData();
  std::unique_ptr<mrdb::ResultSet> result(md->getTables("mrdb_%"));

  if(!result->next()) {
    FAIL("testCanRetrieveTableNames: no result returned, 1 row expected");
  }

  if(result->getString(1) != "mrdb_qry_test") {
    FAIL("testCanRetrieveTableNames: expected table named mrdb_qry_test but " + result->getString(2));
  }

  OK("testCanRetrieveTableNames");

  cleanupFixtures();
}


void testCanRetrieveColumnTypes() {
  buildFixtures();
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  mrdb::DatabaseMetaData* md = conn->getMetaData();

  mrdb::Types::SQLType colType = md->getColumnType("mrdb_qry_test", "id");

  if(colType!=mrdb::Types::INTEGER) {
    FAIL("testCanRetrieveColumnTypes: expected integer type but got "+minerule::Converter(colType).toString());
  }

  OK("testCanRetrieveColumnTypes");

  cleanupFixtures();
}

void testCanGetColumnCount() {

}

void testCanGetColumnName() {

}

void testCanGetColumnType() {

}

void testCanGetColumnTypeName() {

}

void testCanGetPrecision() {

}

void testPrecisionReportedAsZeroWhenNotRelevant() {

}


void testCanGetScale() {

}


int main(int argc, char **argv) {
  mrdb_test::connect = load_mrdb_pg_dylib();

  testCanRetrieveTableNames();
  testCanRetrieveColumnTypes();

  testCanGetColumnCount();
  testCanGetColumnName();
  testCanGetColumnType();
  testCanGetPrecision();
  testCanGetScale();
  testCanGetColumnTypeName();
}
