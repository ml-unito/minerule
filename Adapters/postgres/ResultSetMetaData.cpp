#include "ResultSetMetaData.hpp"
#include "DatabaseMetaData.hpp"
#include "minerule/mrdb/SQLException.hpp"

#include <iostream>

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

std::string ResultSetMetaData::getTableName(int column) {
  Oid tableOid = PQftable(result_, column-1);
  if(tableOid == InvalidOid) {
    throw SQLException("ResultSetMetaData::tableName - Cannot retrieve table id");
  }

  std::string query = "SELECT relname FROM pg_class WHERE oid = " + std::to_string(tableOid);
  PGresult* rs = PQexec(connection_, query.c_str());
  if(PQresultStatus(rs)!=PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(rs);
    throw SQLException("Cannot retrieve table name for oid:"+std::to_string(tableOid)+" - ERROR: "+errorMessage);
  }

  std::string result( PQgetvalue(rs, 0, 0));
  PQclear(rs);

  return result;
}


// @param column the desired column index (starting from 1)
// @return the name of the column SQL type
std::string ResultSetMetaData::getColumnTypeName(int column) {
  std::string query = "SELECT  pg_catalog.format_type(f.atttypid,f.atttypmod) AS type "
        "FROM pg_attribute f "
        "JOIN pg_class c ON c.oid = f.attrelid  "
        "WHERE c.relkind in ('r', 'v') "
        "AND c.relname = '"  + getTableName(column) + "' "
        "AND f.attname = '" + getColumnName(column) + "' ";

  PGresult* rs = PQexec(connection_, query.c_str());
  if(PQresultStatus(rs) != PGRES_TUPLES_OK) {
    throw SQLException("Cannot retrieve column type for table " + getTableName(column) +
        " and column "+getColumnName(column));
  }

  std::string result( PQgetvalue(rs, 0, 0) );
  PQclear(rs);

  return result;
}

}
}
