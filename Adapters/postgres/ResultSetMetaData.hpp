#ifndef MRDB_RESULT_SET_META_DATA_HPP_J3UCH3KS9__
#define MRDB_RESULT_SET_META_DATA_HPP_J3UCH3KS9__

#include <libpq-fe.h>
#include "minerule/mrdb/ResultSetMetaData.hpp"

namespace mrdb {
  namespace postgres {
    class ResultSetMetaData : public mrdb::ResultSetMetaData {
      PGconn* connection_; // weak ref
      PGresult* result_;

      std::string getTableName(int column);
    public:
      ResultSetMetaData(PGconn* connection, PGresult* result) : connection_(connection), result_(result) {}
      virtual ~ResultSetMetaData() {}

      // @return the number of columns of a result set
      virtual int getColumnCount () const;

      // @param column the desired column index (starting from 1)
      // @return the name of the specified column
      virtual std::string getColumnName(int column) const;

      // @param column the desired column index (starting from 1)
      // @return the column type of the specified column.
      virtual int getColumnType(int column) const;

      // @param column the desired column index (starting from 1)
      // @return the column precision of the specified column.
      virtual int getPrecision (int column) const;

      // @param column the desired column index (starting from 1)
      // @return the column scale of the specified column.
      virtual int getScale (int column) const;

      // @param column the desired column index (starting from 1)
      // @return the name of the column SQL type
      virtual std::string getColumnTypeName (int column);
    };
  }
}


#endif /* end of include guard: MRDB_RESULT_SET_META_DATA_HPP_J3UCH3KS9__ */
