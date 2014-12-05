#ifndef MRDB_RESULTSETMETADATA_HPP_3MNCH3KCU32B__
#define MRDB_RESULTSETMETADATA_HPP_3MNCH3KCU32B__

#include <string>

namespace mrdb {
class ResultSetMetaData {
public:
  virtual ~ResultSetMetaData() {}

  /// @return the number of columns of a result set
  virtual int getColumnCount () const = 0;

  /// @param column the desired column index (starting from 1)
  /// @return the name of the specified column
  virtual std::string getColumnName(int column) const = 0;

  /// @param column the desired column index (starting from 1)
  /// @return the column type of the specified column.
  virtual int getColumnType(int column) const = 0;

  /// @param column the desired column index (starting from 1)
  /// @return the column precision of the specified column.
  virtual int getPrecision (int column) const = 0;

  /// @param column the desired column index (starting from 1)
  /// @return the column scale of the specified column.
  virtual int getScale (int column) const = 0;

  /// @param column the desired column index (starting from 1)
  /// @return the name of the column SQL type
  virtual std::string getColumnTypeName (int column) = 0;

};
}

#endif /* end of include guard: MRDB_RESULTSETMETADATA_HPP_3MNCH3KCU32B__ */
