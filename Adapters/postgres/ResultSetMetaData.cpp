#include "ResultSetMetaData.hpp"
#include "DatabaseMetaData.hpp"
#include "minerule/mrdb/SQLException.hpp"
#include "minerule/Utils/Converter.hpp"

namespace mrdb {
namespace postgres {
// @return the number of columns of a result set
int ResultSetMetaData::getColumnCount() const {
  return PQnfields(result_);
}


// @param column the desired column index (starting from 1)
// @return the name of the specified column
std::string ResultSetMetaData::getColumnName(int column) const {
  return PQfname(result_, column-1);
}


// @param column the desired column index (starting from 1)
// @return the column type of the specified column.
int ResultSetMetaData::getColumnType(int column) const {
  return DatabaseMetaData::sqlType(PQftype(result_, column-1));
}


// @param column the desired column index (starting from 1)
// @return the column precision of the specified column.
int ResultSetMetaData::getPrecision(int column) const {
  int typeMod = PQfmod(result_, column-1);
  if( typeMod == -1 ) {
    return 0;
  }

  return ((typeMod - 4) >> 16) & 0xffff;
}


// @param column the desired column index (starting from 1)
// @return the column scale of the specified column.
int ResultSetMetaData::getScale(int column) const {
  int typeMod = PQfmod(result_, column-1);
  if( typeMod == -1 ) {
    return 0;
  }

  return (typeMod - 4) & 0xffff;
}


// @param column the desired column index (starting from 1)
// @return the name of the column SQL type
std::string ResultSetMetaData::getColumnTypeName(int column) {
  return DatabaseMetaData::typeName(PQftype(result_, column-1));
}

}
}
