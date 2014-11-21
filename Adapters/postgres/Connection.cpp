#include <minerule/Utils/MineruleException.hpp>
#include <minerule/Utils/Converter.hpp>

#include "Connection.hpp"
#include "Statement.hpp"
#include "PreparedStatement.hpp"
#include "DatabaseMetaData.hpp"

#include <iostream>

namespace mrdb {
namespace postgres {

Connection::Connection( const std::string &db,
                        const std::string &user,
                        const std::string &pwd) : metadata_(NULL) {
  std::string connectionString =
      "host=localhost dbname=" + db + " user=" + user + " password=" + pwd;

  connection_ = PQconnectdb(connectionString.c_str());

  if (PQstatus(connection_) != CONNECTION_OK) {
    std::string msg(PQerrorMessage(connection_));
    PQfinish(connection_);
    connection_ = NULL;

    throw mrdb::SQLException(msg);
  }
}

Connection::~Connection() {
  if (connection_ != NULL)
    PQfinish(connection_);

  if(metadata_ != NULL) {
    delete metadata_;
  }
}

mrdb::Statement *Connection::createStatement() throw(mrdb::SQLException&) {
  return new mrdb::postgres::Statement(connection_);
}

mrdb::PreparedStatement *Connection::prepareStatement(const std::string& sql) throw(mrdb::SQLException&) {
  return new mrdb::postgres::PreparedStatement(connection_, sql);
}

mrdb::DatabaseMetaData* Connection::getMetaData() {
  if(metadata_==NULL) {
    metadata_ = new mrdb::postgres::DatabaseMetaData(connection_);
  }

  return metadata_;
}


} // namespace postgres
} // namespace mrdb
