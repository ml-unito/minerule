#define CATCH_CONFIG_MAIN

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

#include "../catch.hpp"

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


TEST_CASE("Result Set metadata") {
  mrdb_test::connect = load_mrdb_pg_dylib();
  buildFixtures();

  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  std::unique_ptr<mrdb::Statement> state(conn->createStatement());
  std::unique_ptr<mrdb::ResultSet> rs(state->executeQuery("SELECT * FROM mrdb_qry_test"));
  mrdb::ResultSetMetaData* md = rs->getMetaData();

  SECTION("Column count") {
    REQUIRE(md->getColumnCount() == 3);
  }

  SECTION("Column name") {
    REQUIRE(md->getColumnName(1) == "id");
  }

  SECTION("Column type") {
    REQUIRE(md->getColumnType(1) == mrdb::Types::NUMERIC);
  }

  SECTION("Column type name") {
    REQUIRE(md->getColumnTypeName(1) == "numeric");
  }

  SECTION("Get precision") {
    REQUIRE(md->getPrecision(1) == 10);
  }

  SECTION("Get precision reports zero when not relevant") {
    REQUIRE(md->getPrecision(2) == 0);
  }

  SECTION("Get scale") {
    REQUIRE(md->getScale(1) == 2);
  }

  cleanupFixtures();
}
