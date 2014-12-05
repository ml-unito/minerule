#ifndef MRDB_POSTGRES_PREPARED_STATEMENT_H_JKDNCJMLLIE__
#define MRDB_POSTGRES_PREPARED_STATEMENT_H_JKDNCJMLLIE__

#include <libpq-fe.h>
#include <minerule/mrdb/PreparedStatement.hpp>
#include <minerule/mrdb/ResultSet.hpp>

#include <vector>
#include <string>

namespace mrdb {
namespace postgres {

class PreparedStatement : public mrdb::PreparedStatement {
private:
  PGconn *connection_;
  std::string statementName_;
  int numParams_;
  char **params_;

  std::pair<int, std::string> formatParams(std::string sql) const;
  void initParams();
  void setParam(int, const std::string&);
  void freeParams();
public:
  PreparedStatement(PGconn* connection_, std::string sql) throw(mrdb::SQLException&);
  virtual ~PreparedStatement() { freeParams(); }

  virtual bool execute() throw(mrdb::SQLException&);
  virtual mrdb::ResultSet* executeQuery() throw(mrdb::SQLException&);

  virtual void setDouble (int idx, double val);
  virtual void setInt (int idx, int val);
  virtual void setLong(int idx, long val);
  virtual void setString (int idx, const std::string &val);

};
}
}

#endif /* end of include guard:                                                \
          MRDB_POSTGRES_PREPARED_STATEMENT_H_JKDNCJMLLIE__ */
