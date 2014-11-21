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
  result = PQexec(connection, "CREATE TABLE mrdb_qry_test (id numeric(10,2), str varchar, flt float)");
  if(PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error creating fixture table" << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  result = PQexec(connection,"INSERT INTO mrdb_qry_test (id,str,flt) VALUES (12.3, 'abc', 4.5), (2,'def', 5.4)");
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

#define BUILD_META_DATA_OBJ \
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", "")); \
  std::unique_ptr<mrdb::Statement> state(conn->createStatement()); \
  std::unique_ptr<mrdb::ResultSet> rs(state->executeQuery("SELECT * FROM mrdb_qry_test")); \
  mrdb::ResultSetMetaData* md = rs->getMetaData()



void testCanGetColumnCount() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getColumnCount() != 3) {
    FAIL("testCanGetColumnCount: expected 3, but got "+minerule::Converter(md->getColumnCount()).toString());
  }

  OK("testCanGetColumnCount");

  cleanupFixtures();
}

void testCanGetColumnName() {
  buildFixtures();
  BUILD_META_DATA_OBJ;

  if(md->getColumnName(1) != "id") {
    FAIL("testCanGetColumnName: expected 'id' but got "+md->getColumnName(1));
  }

  OK("testCanGetColumnName");

  cleanupFixtures();
}

void testCanGetColumnType() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getColumnType(1) != mrdb::Types::NUMERIC) {
    FAIL("testCanGetColumnType: expected numeric type, but got " +
      minerule::Converter(md->getColumnType(1)).toString());
  }

  OK("testCanGetColumnType");

  cleanupFixtures();
}

void testCanGetColumnTypeName() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getColumnTypeName(1) != "numeric") {
    FAIL("testCanGetColumnTypeName: expected numeric type, but got " +
    md->getColumnTypeName(1));
  }

  OK("testCanGetColumnTypeName");

  cleanupFixtures();
}

void testCanGetPrecision() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getPrecision(1) != 10) {
    FAIL("testCanGetPrecision: expected 10, but got " +
      minerule::Converter(md->getPrecision(1)).toString());
  }

  OK("testCanGetPrecision");

  cleanupFixtures();
}

void testPrecisionReportedAsZeroWhenNotRelevant() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getPrecision(2) != 0) {
    FAIL("testPrecisionReportedAsZeroWhenNotRelevant: expected 0, but got " +
    minerule::Converter(md->getPrecision(2)).toString());
  }

  OK("testPrecisionReportedAsZeroWhenNotRelevant");

  cleanupFixtures();
}


void testCanGetScale() {
  buildFixtures();

  BUILD_META_DATA_OBJ;

  if(md->getScale(1) != 2) {
    FAIL("testCanGetScale: expected 2, but got " +
    minerule::Converter(md->getScale(1)).toString());
  }

  OK("testCanGetScale");

  cleanupFixtures();
}


int main(int argc, char **argv) {
  mrdb_test::connect = load_mrdb_pg_dylib();

  testCanGetColumnCount();
  testCanGetColumnName();
  testCanGetColumnType();
  testCanGetPrecision();
  testPrecisionReportedAsZeroWhenNotRelevant();
  testCanGetScale();
  testCanGetColumnTypeName();
}
