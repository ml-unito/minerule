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

MRDBConnectFun load_mrdb_pg_dylib() {
  void *handle = dlopen("libmrdb_pg.so", RTLD_LAZY);

  if (handle == NULL) {
    std::cout << "Error:" << dlerror() << std::endl;
    exit(1);
  }

  MRDBConnectFun connect = (MRDBConnectFun)dlsym(handle, "connect");

  if (connect == NULL) {
    std::cout << "Error:" << dlerror() << std::endl;
    exit(1);
  }

  return connect;
}


void buildFixtures() {
  PGconn *connection = PQconnectdb("host=localhost user=pgtest");

  if (PQstatus(connection) != CONNECTION_OK) {
    std::cerr << "Error connecting to the database:"
    << PQerrorMessage(connection) << std::endl;
    exit(1);
  }

  PGresult *result;

  result = PQexec(connection, "DROP TABLE IF EXISTS mrdb_qry_test");
  result = PQexec(connection, "CREATE TABLE mrdb_qry_test (id int, str "
  "varchar, flt float, chr character(25))");
  if (PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error creating fixture table"
    << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  result =
  PQexec(connection, "INSERT INTO mrdb_qry_test (id,str,flt, chr) VALUES "
  "(1, 'abc', 4.5, 'ABC'), (2,'def', 5.4, 'DEF')");
  if (PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error populating fixture table"
    << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  PQfinish(connection);
}

void cleanupFixtures() {
  PGconn *connection = PQconnectdb("host=localhost user=pgtest");

  if (PQstatus(connection) != CONNECTION_OK) {
    std::cerr << "Error connecting to the database:"
    << PQerrorMessage(connection) << std::endl;
    exit(1);
  }

  PGresult *result = PQexec(connection, "DROP TABLE mrdb_qry_test;");
  if (PQresultStatus(result) != PGRES_COMMAND_OK) {
    std::cout << "Error dropping fixture table"
    << PQresultErrorMessage(result) << std::endl;
    exit(1);
  }
  PQclear(result);

  PQfinish(connection);
}

TEST_CASE("Thread safety") {
  REQUIRE(PQisthreadsafe() == 1);
}

TEST_CASE("Connection") {
  mrdb_test::connect = load_mrdb_pg_dylib();

  SECTION("Can connect") {
    REQUIRE_NOTHROW(std::unique_ptr<mrdb::Connection> conn(
      mrdb_test::connect("pgtest", "pgtest", ""));
    );
  }

  SECTION("Raises an exception when connection fails") {
    REQUIRE_THROWS(
      std::unique_ptr<mrdb::Connection> conn(
        mrdb_test::connect("bad-db-name", "no-user", "nopwd"));

      conn->createStatement();
    );
  }

}

TEST_CASE("Statements work correctly") {

  mrdb_test::connect = load_mrdb_pg_dylib();
  buildFixtures();

  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  SECTION("Can execute query") {
    std::unique_ptr<mrdb::Statement> statement(conn->createStatement());
    std::unique_ptr<mrdb::ResultSet> rs(
        statement->executeQuery("SELECT id,str,flt,chr FROM mrdb_qry_test"));

    REQUIRE(rs);
    REQUIRE(rs->next());
    REQUIRE(1 == rs->getInt(1));
    REQUIRE("abc" == rs->getString(2));
    REQUIRE(4.5 == rs->getDouble(3));
    REQUIRE( "ABC                      " == rs->getString(4) );
    REQUIRE(rs->next());
    REQUIRE(2 == rs->getInt(1));
    REQUIRE("def" == rs->getString(2));
    REQUIRE(5.4 == rs->getDouble(3));
    REQUIRE( "DEF                      " == rs->getString(4) );
    REQUIRE(!rs->next());
  }


  SECTION("Can execute prepared commands") {
    std::unique_ptr<mrdb::PreparedStatement> pstate(
        conn->prepareStatement("INSERT INTO mrdb_qry_test VALUES (?,?,?)"));
    std::unique_ptr<mrdb::Statement> qstate(conn->createStatement());

    pstate->setInt(1, 3);
    pstate->setString(2, "ghi");
    pstate->setDouble(3, 54.0);

    std::unique_ptr<mrdb::ResultSet> rs1(
      qstate->executeQuery("SELECT count(*) FROM mrdb_qry_test"));

    REQUIRE(rs1->next());
    REQUIRE(rs1->getInt(1) == 2);

    pstate->execute();

    std::unique_ptr<mrdb::ResultSet> rs2(
      qstate->executeQuery("SELECT count(*) FROM mrdb_qry_test"));

    REQUIRE(rs2->next());
    REQUIRE(rs2->getInt(1) == 3);
  }


  SECTION("Can execute prepared queries") {
    std::unique_ptr<mrdb::PreparedStatement> pstate(conn->prepareStatement(
        "SELECT * FROM mrdb_qry_test WHERE id=? OR str=?"));

    pstate->setInt(1, 1);
    pstate->setString(2, "def");

    std::unique_ptr<mrdb::ResultSet> rs(pstate->executeQuery());

    REQUIRE(rs->next());
    REQUIRE(rs->getInt(1) == 1);
    REQUIRE(rs->next());
    REQUIRE(rs->getInt(1) == 2);
    REQUIRE(!rs->next());
  }

  SECTION("Can execute commands") {
    std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

    REQUIRE_NOTHROW(statement->execute("CREATE TABLE mrdb_test (id int, str varchar)"));
    REQUIRE_NOTHROW(statement->execute("INSERT INTO mrdb_test (id,str) VALUES (1, 'test')"));
    REQUIRE_NOTHROW(statement->execute("DROP TABLE mrdb_test"));
  }

  SECTION("Raises an excetpion when commands fail") {
    std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

    REQUIRE_THROWS(statement->execute("CREATE MICKEY AND GOOFY"));
  }

  SECTION("Raises an excetpion when query fail") {
    std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

    REQUIRE_THROWS(statement->execute("SECT id,str,flt FROM mrdb_qry_test"));
  }

  SECTION("Raises an excetpion when prepared command fail") {
    std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

    REQUIRE_THROWS(
      std::unique_ptr<mrdb::PreparedStatement> pstate(
          conn->prepareStatement("INRT INTO mrdb_qry_test VALUES (?,?,?)"));
    );
  }

  cleanupFixtures();
}
