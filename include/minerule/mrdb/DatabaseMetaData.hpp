#ifndef MRDB_DATABASE_META_DATA_HPP_J33CHCYHY__
#define MRDB_DATABASE_META_DATA_HPP_J33CHCYHY__

#include <string>

#include "ResultSet.hpp"
#include "minerule/mrdb/Types.hpp"

namespace mrdb {
  class DatabaseMetaData {
  public:
    virtual ~DatabaseMetaData () {}

    /// Searches the current db for given table names.
    /// @param tableNamePattern a pattern to be used to filter the result
    /// @result a ResultSet with a row for each found table, first column of the result
    ///    contains the name of the found table.
    /// Usage example: getTables("mr_%") --> retuns all tables having a name starting with mr_
    virtual ResultSet* getTables(const std::string& tableNamePattern) = 0;

    /// @param tableName the table name to query
    /// @param columnName the column name whose type is sought
    /// @return the SQLType of the given column for the given table
    virtual Types::SQLType getColumnType(const std::string& tableName, const std::string& columnName) = 0;

    /// @return a ResultSet containing a table with three fields:
    ///   field1: TABLE_NAME is a table name
    ///   field2: COLUMN_NAME is a column name in <TABLE_NAME>
    ///   field3: TYPE_NAME is the type name of column <COLUMN_NAME>//
    ///  results are sorted by TABLE_NAME
    virtual ResultSet* getColumns() = 0;
  };
}


#endif /* end of include guard: MRDB_DATABASE_META_DATA_HPP_J33CHCYHY__ */
