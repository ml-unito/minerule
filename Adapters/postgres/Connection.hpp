#ifndef MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__
#define MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__

#include <libpq-fe.h>
#include <minerule/mrdb/Connection.hpp>
#include <minerule/mrdb/SQLException.hpp>

namespace mrdb {
namespace postgres {
class Connection : public mrdb::Connection {
private:
  std::string db_;
  std::string user_;
  std::string pwd_;

  PGconn* connect() const;
public:
  Connection(const std::string &db, const std::string &user, const std::string &pwd) :
    db_(db), user_(user), pwd_(pwd) {};
  virtual ~Connection() {};

  virtual mrdb::Statement *createStatement();
  virtual mrdb::PreparedStatement *prepareStatement(const std::string &sql);

  virtual DatabaseMetaData* getMetaData();
};
}
}

#endif /* end of include guard: MRDB_POSTGRES_CONNECTION_H_KJ3KJJ3943C__ */
