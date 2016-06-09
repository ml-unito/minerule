#include <minerule/Utils/Converter.hpp>

#include "Connection.hpp"
#include "Statement.hpp"
#include "PreparedStatement.hpp"
#include "DatabaseMetaData.hpp"

#include <iostream>

namespace mrdb {
namespace postgres {

mrdb::Statement *Connection::createStatement() {
  return new mrdb::postgres::Statement(connection_);
}

mrdb::PreparedStatement *Connection::prepareStatement(const std::string& sql) {
  return new mrdb::postgres::PreparedStatement(connection_, sql);
}

mrdb::DatabaseMetaData* Connection::getMetaData() {
  return new mrdb::postgres::DatabaseMetaData(connection_);
}


} // namespace postgres
} // namespace mrdb
