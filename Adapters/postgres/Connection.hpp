#ifndef MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__
#define MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__

#include <libpq-fe.h>
#include <minerule/mrdb/Connection.hpp>
#include <minerule/mrdb/SQLException.hpp>

namespace mrdb {
namespace postgres {
class Connection : public mrdb::Connection {
private:
  PGconn* connection_;

public:
  Connection(const std::string &db, const std::string &user, const std::string &pwd){
    std::string connectionString =
        "host=localhost dbname=" + db + " user=" + user + " password=" + pwd;

    connection_ = PQconnectdb(connectionString.c_str());

    if (PQstatus(connection_) != CONNECTION_OK) {
      std::string msg(PQerrorMessage(connection_));
      PQfinish(connection_);
      connection_ = NULL;

      throw mrdb::SQLException(msg);
    }
  };
  virtual ~Connection() { PQfinish(connection_); };

  virtual mrdb::Statement *createStatement();
  virtual mrdb::PreparedStatement *prepareStatement(const std::string &sql);

  virtual DatabaseMetaData* getMetaData();
};
}
}

#endif /* end of include guard: MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__ */
