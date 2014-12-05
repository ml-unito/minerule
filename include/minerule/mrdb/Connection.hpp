#ifndef MRDB_CONNECTION_H_KEKCN3MCK2__
#define MRDB_CONNECTION_H_KEKCN3MCK2__

#include "minerule/Utils/MineruleException.hpp"
#include "Statement.hpp"
#include "PreparedStatement.hpp"
#include "SQLException.hpp"
#include "DatabaseMetaData.hpp"

namespace mrdb {

class Connection {
public:
  virtual ~Connection() {}

  /// @return a newly created Statement.
  /// @throws a MineruleException with an error code MR_ERROR_DATABASE_ERROR in
  /// case
  /// the creation fails.
  /// @memory the returned object needs to be dealloced by the user once no
  /// longer needed.
  virtual Statement *createStatement() throw(mrdb::SQLException &) = 0;

  /// @param sql the sql command to be prepared. Parameters need to be specified
  /// using question marks, e.g., SELECT * FROM table WHERE id=?
  /// @return a newly created PreparedStatement.
  /// @throws a MineruleException with an error code MR_ERROR_DATABASE_ERROR in
  /// case
  /// the creation fails.
  /// @memory the returned object needs to be dealloced by the user once no
  /// longer needed.
  virtual PreparedStatement *prepareStatement(const std::string &sql) throw(
      mrdb::SQLException &) = 0;

  /// @return the database meta data for this connection
  /// The returned object is owned by this class. The user
  /// must not release it. It will be freed along with this
  /// connection.
  virtual DatabaseMetaData *getMetaData() = 0;
};

} // namespace


// The MRDBConnectFun type represents factory functions for mrdb::Connection
// objects. This is the type of the entry point of dynamic libraries implementing
// mrdb adapters.
extern "C" {
typedef mrdb::Connection *(*MRDBConnectFun)(const char*,
                                            const char*,
                                            const char*);
}

#endif /* end of include guard: MRDB_CONNECTION_H_KEKCN3MCK2__ */
