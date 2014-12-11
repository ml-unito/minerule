#ifndef MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__
#define MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__

#include <libpq-fe.h>
#include <minerule/mrdb/Connection.hpp>
#include <minerule/mrdb/SQLException.hpp>

namespace mrdb {
namespace postgres {
class Connection : public mrdb::Connection {
private:
  PGconn *connection_;
  DatabaseMetaData* metadata_;

  // std::string formatParams(std::string sql) const;
public:
  Connection(const std::string &db, const std::string &user, const std::string &pwd);
  virtual ~Connection();

  virtual mrdb::Statement *createStatement();
  virtual mrdb::PreparedStatement *prepareStatement(const std::string &sql);

  virtual DatabaseMetaData* getMetaData();
};
}
}

#endif /* end of include guard: MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__ */
