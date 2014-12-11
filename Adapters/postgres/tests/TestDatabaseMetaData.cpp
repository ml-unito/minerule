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

TEST_CASE("Database metadata") {
  mrdb_test::connect = load_mrdb_pg_dylib();
  buildFixtures();
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  SECTION("Can retrieve table names") {
    mrdb::DatabaseMetaData* md = conn->getMetaData();
    std::unique_ptr<mrdb::ResultSet> result(md->getTables("mrdb_%"));

    REQUIRE_NOTHROW(
      result->next();
      REQUIRE( result->getString(1) == "mrdb_qry_test")
    );
  }

  SECTION("Can retrieve column types") {
    mrdb::DatabaseMetaData* md = conn->getMetaData();
    mrdb::Types::SQLType colType = md->getColumnType("mrdb_qry_test", "id");

    REQUIRE(colType==mrdb::Types::INTEGER);

  }

  cleanupFixtures();
}
