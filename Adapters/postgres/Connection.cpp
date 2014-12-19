#include <minerule/Utils/Converter.hpp>

#include "Connection.hpp"
#include "Statement.hpp"
#include "PreparedStatement.hpp"
#include "DatabaseMetaData.hpp"

#include <iostream>

namespace mrdb {
namespace postgres {

PGconn* Connection::connect() const {
  std::string connectionString =
      "host=localhost dbname=" + db_ + " user=" + user_ + " password=" + pwd_;

  PGconn* connection = PQconnectdb(connectionString.c_str());

  if (PQstatus(connection) != CONNECTION_OK) {
    std::string msg(PQerrorMessage(connection));
    PQfinish(connection);
    connection = NULL;

    throw mrdb::SQLException(msg);
  }

  return connection;
}

mrdb::Statement *Connection::createStatement() {
  return new mrdb::postgres::Statement(connect());
}

mrdb::PreparedStatement *Connection::prepareStatement(const std::string& sql) {
  return new mrdb::postgres::PreparedStatement(connect(), sql);
}

mrdb::DatabaseMetaData* Connection::getMetaData() {
  return new mrdb::postgres::DatabaseMetaData(connect());
}


} // namespace postgres
} // namespace mrdb
