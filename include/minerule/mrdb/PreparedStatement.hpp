#ifndef MRDB_PREPARED_STATEMENT_H_K3I3JD82G__
#define MRDB_PREPARED_STATEMENT_H_K3I3JD82G__

#include "ResultSet.hpp"
#include "SQLException.hpp"
#include "minerule/Utils/MineruleException.hpp"

namespace mrdb {

  class PreparedStatement {
  public:
    virtual ~PreparedStatement() {}

    /// Execute this statement (assumes that it does not returns
    /// any tuple)
    /// @return true if a result set was available, false otherwise
    /// @throws a MineruleException (with error code MR_ERROR_DATABASE_ERROR) if
    ///   anything goes wrong.
    virtual bool execute() = 0;

    /// Execute this statement.
    /// THREAD SAFETY: It returns a ResultSet that should be consumed by the same
    ///                thread that owns this object.
    /// @return a ResultSet
    /// @throws a MineruleException (with error code MR_ERROR_DATABASE_ERROR) if
    ///   anything goes wrong.
    /// @memory the caller should dealloc the result set once it is no longer needed
    virtual ResultSet* executeQuery() = 0;

    /// Sets a parameter value to a Double
    /// @param idx the index of the parameter to be set
    /// @param val the value to set
    virtual void setDouble (int idx, double val) = 0;

    /// Sets a parameter value to an int
    /// @param idx the index of the parameter to be set
    /// @param val the value to set
    virtual void setInt (int idx, int val) = 0;

    /// Sets a parameter value to a string
    /// @param idx the index of the parameter to be set
    /// @param val the value to set
    virtual void setString (int idx, const std::string &val) = 0;

    /// Sets a parameter value to a Long
    /// @param idx the index of the parameter to be set
    /// @param val the value to set
    virtual void setLong (int idx, long val) = 0;
  };

}

#endif /* end of include guard: MRDB_PREPARED_STATEMENT_H_K3I3JD82G */
