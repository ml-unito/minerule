#ifndef MRDB_STATEMENT_H_KDJNMCKELKAJC__
#define MRDB_STATEMENT_H_KDJNMCKELKAJC__

#include "minerule/Utils/MineruleException.hpp"
#include "ResultSet.hpp"
#include "SQLException.hpp"


namespace mrdb {

class Statement {
public:
  virtual ~Statement() {}

  /// Executes the given query and build the correspondig result set.
  /// @return the ResultSet
  /// @memory the caller is responsible of deallocing the result set once it is no longer
  ///   needed.
  virtual ResultSet *executeQuery(const std::string &sql) throw(mrdb::SQLException&) = 0;

  /// Execute the given query.
  /// @returns true if a result set was available, false otherwise
  /// and false otherwise.
  /// @throw a MineruleException (with error code MR_ERROR_DATABASE_ERROR) in case something
  ///  goes wrong.
  virtual bool execute(const std::string &sql) throw(mrdb::SQLException&) = 0;
};

}

#endif /* end of include guard: MRDB_STATEMENT_H_KDJNMCKELKAJC__ */
