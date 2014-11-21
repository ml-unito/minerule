#ifndef MRDB_POSTGRES_STATEMENT_H_KIBCH3KZL__
#define MRDB_POSTGRES_STATEMENT_H_KIBCH3KZL__

#include <libpq-fe.h>
#include <minerule/mrdb/Statement.hpp>

namespace mrdb {

namespace postgres {
class Statement : public mrdb::Statement {
private:
  PGconn* connection_; // weak reference to the connection
public:
  Statement(PGconn* connection) : connection_(connection) {}
  virtual ~Statement() {}

  virtual mrdb::ResultSet *executeQuery(const std::string &sql) throw(mrdb::SQLException&);
  virtual bool execute(const std::string &sql) throw(mrdb::SQLException&);
};

} // namespace postgres
} // namespace mrdb

#endif /* end of include guard: MRDB_POSTGRES_STATEMENT_H_KIBCH3KZL__ */
