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


void testCanConnect() {
  try {
    std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  } catch (std::exception &e) {
    FAIL(e.what());
  }

  OK("testCanConnect");
}

void testRaisesExceptionWhenItCannotConnect() {
  try {
    std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("bad-db-name", "no-user", "nopwd"));
      FAIL("can connect even with bad credentials");
    } catch (std::exception &e) {
  }

  OK("testRaisesExceptionWhenItCannotConnect");
}

void checkStatement(const std::unique_ptr<mrdb::Statement>& statement, std::string name,std::string sql) {
  try {
    statement->execute(sql);
    OK(std::string("testCanExecuteCommand " + name));
  } catch (std::exception &e) {
    FAIL(e.what());
  }
}

void testCanExecuteCommand() {
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

  checkStatement(statement, "CREATE", "CREATE TABLE mrdb_test (id int, str varchar)");
  checkStatement(statement, "INSERT", "INSERT INTO mrdb_test (id,str) VALUES (1, 'test')");
  checkStatement(statement, "DROP", "DROP TABLE mrdb_test");
}

void testRaisesAnExceptionWhenCommandFails() {
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

  try {
    statement->execute("CREATE MICKEY AND GOOFY");
    FAIL("testRaisesAnExceptionWhenCommandFails: Execution of bogus cmd did not throw an exception");
  } catch( std::exception& e ) {
    OK("testRaisesAnExceptionWhenCommandFails");
  }
}

void testCanExecuteQuery() {
  buildFixtures();
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  std::unique_ptr<mrdb::Statement> statement(conn->createStatement());


  std::unique_ptr<mrdb::ResultSet> rs(statement->executeQuery("SELECT id,str,flt FROM mrdb_qry_test"));
  assert(rs!=NULL);
  assert(rs->next());
  assert( 1 == rs->getInt(1) );
  assert( "abc" == rs->getString(2) );
  assert( 4.5 == rs->getDouble(3) );
  assert(rs->next());
  assert( 2 == rs->getInt(1) );
  assert( "def" == rs->getString(2) );
  assert( 5.4 == rs->getDouble(3) );
  assert(!rs->next());

  OK("testExecuteQuery");
  cleanupFixtures();
}

void testRaisesAnExceptionWhenQueryFails() {
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  std::unique_ptr<mrdb::Statement> statement(conn->createStatement());

  try {
    std::unique_ptr<mrdb::ResultSet> rs(statement->executeQuery("SECT id,str,flt FROM mrdb_qry_test"));
    FAIL("testRaisesAnExceptionWhenQueryFails: Execution of bogus query did not throw an exception");
  } catch( std::exception& e ) {
    OK("testRaisesAnExceptionWhenQueryFails");
  }
}


void testCanExecutePreparedCommand() {
  buildFixtures();

  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  std::unique_ptr<mrdb::PreparedStatement> pstate(conn->prepareStatement("INSERT INTO mrdb_qry_test VALUES (?,?,?)"));

  pstate->setInt(1,3);
  pstate->setString(2, "ghi");
  pstate->setDouble(3, 54.0);

  pstate->execute();

  std::unique_ptr<mrdb::Statement> qstate(conn->createStatement());
  std::unique_ptr<mrdb::ResultSet> rs(qstate->executeQuery("SELECT count(*) FROM mrdb_qry_test"));

  rs->next();
  if(rs->getInt(1) != 3) {
    std::string errorMessage = std::string("testCanExecutePreparedCommand: expected 3"
      " rows in dataset, but ") + minerule::Converter(rs->getInt(2)).toString() + "found";
    FAIL(errorMessage);
  }


  OK("testCanExecutePreparedCommand");

  cleanupFixtures();
}

void testRaisesAnExceptionWhenPreparedCommandFails() {
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  try {
    std::unique_ptr<mrdb::PreparedStatement> pstate(conn->prepareStatement("INRT INTO mrdb_qry_test VALUES (?,?,?)"));
    FAIL("testCanExecutePreparedCommand: successfully execute bogus command");
  } catch (std::exception& e){
    OK("testCanExecutePreparedCommand");
  }
}


void testCanExecutePreparedQueries() {
  buildFixtures();

  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));
  std::unique_ptr<mrdb::PreparedStatement> pstate(conn->prepareStatement("SELECT * FROM mrdb_qry_test WHERE id=? OR str=?"));

  pstate->setInt(1,1);
  pstate->setString(2, "def");

  std::unique_ptr<mrdb::ResultSet> rs(pstate->executeQuery());

  assert(rs->next());
  assert( rs->getInt(1) == 1 );
  assert(rs->next());
  assert( rs->getInt(1) == 2 );
  assert( !rs->next() );

  OK("testCanExecutePreparedQuery");

  cleanupFixtures();
}

void testRaisesAnExcepionWhenPreparedQueriesFails() {
  std::unique_ptr<mrdb::Connection> conn(mrdb_test::connect("pgtest", "pgtest", ""));

  try {
    std::unique_ptr<mrdb::PreparedStatement> pstate(conn->prepareStatement("SLT * FROM mrdb_qry_test WHERE id=? OR str=?"));
    FAIL("testRaisesAnExcepionWhenPreparedQueriesFails: successfully execute bogus query");
  } catch( std::exception& e ) {
    OK("testRaisesAnExcepionWhenPreparedQueriesFails");
  }
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


int main(int argc, char **argv) {

  mrdb_test::connect = load_mrdb_pg_dylib();

  testCanConnect();
  testRaisesExceptionWhenItCannotConnect();

  testCanExecuteCommand();
  testRaisesAnExceptionWhenCommandFails();

  testCanExecuteQuery();
  testRaisesAnExceptionWhenQueryFails();

  testCanExecutePreparedCommand();
  testRaisesAnExceptionWhenPreparedCommandFails();

  testCanExecutePreparedQueries();
  testRaisesAnExcepionWhenPreparedQueriesFails();
}
