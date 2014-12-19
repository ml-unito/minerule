#ifndef MRDB_DATABASE_META_DATA_HPP_J3C83K9D43K__
#define MRDB_DATABASE_META_DATA_HPP_J3C83K9D43K__

#include <libpq-fe.h>
#include <string>

#include <minerule/mrdb/ResultSet.hpp>
#include <minerule/mrdb/Types.hpp>
#include <minerule/mrdb/DatabaseMetaData.hpp>

namespace mrdb {
namespace postgres {
class DatabaseMetaData : public mrdb::DatabaseMetaData {
  PGconn *connection_; // strong ref
public:
  DatabaseMetaData(PGconn* connection) : connection_(connection) {};
  virtual ~DatabaseMetaData() { PQfinish(connection_); }

  virtual mrdb::ResultSet* getTables(const std::string& tableNamePattern);
  virtual mrdb::Types::SQLType getColumnType(const std::string& tableName, const std::string& columnName);

  // Returns the SQL type associated with the given oid
  // it works only with standard SQL types, it does not support
  // composite or user types.
  static Types::SQLType sqlType(int oid);

  // Returns the postgres type name associated with the given oid
  // it works only with standard SQL types, it does not support
  // composite or user types.
  static std::string typeName(int oid);


  virtual ResultSet* getColumns();

};

} // namespace postgres
} // namespace mrdb

#endif /* end of include guard: MRDB_DATABASE_META_DATA_HPP_J3C83K9D43K__ */
