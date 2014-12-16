#include "DatabaseMetaData.hpp"
#include "minerule/mrdb/SQLException.hpp"
#include "ResultSet.hpp"

#include <postgres.h>
#include <catalog/pg_type.h>
#include <string>


namespace mrdb {
namespace postgres {

mrdb::ResultSet *
DatabaseMetaData::getTables(const std::string &tableNamePattern) {
  std::string qry = "SELECT tablename FROM pg_tables WHERE tablename like '"+tableNamePattern+"'";
  PGresult *result = PQexec( connection_, qry.c_str());

  if(PQresultStatus(result)!=PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);

    throw mrdb::SQLException(errorMessage);
  }

  return new mrdb::postgres::ResultSet(connection_, result);
}

Types::SQLType DatabaseMetaData::sqlType(int oid) {
  switch(oid) {
    case INT8OID:         return mrdb::Types::BIGINT;
    case BYTEAOID:        return mrdb::Types::BINARY;
    case BITOID:          return mrdb::Types::BIT;
    case CHAROID:         return mrdb::Types::CHAR;
    case DATEOID:         return mrdb::Types::DATE;
    case FLOAT8OID:       return mrdb::Types::DOUBLE;
    case FLOAT4OID:       return mrdb::Types::FLOAT; // aka mrdb::Types::REAL
    case INT4OID:         return mrdb::Types::INTEGER;
    case NUMERICOID:      return mrdb::Types::NUMERIC; // aka mrdb::Types::DECIMAL;
    case INT2OID:         return mrdb::Types::SMALLINT;
    case TIMEOID:         return mrdb::Types::TIME;
    case TIMESTAMPOID:    return mrdb::Types::TIMESTAMP;
    case BPCHAROID:       return mrdb::Types::CHAR;
    case TEXTOID:
    case VARCHAROID:      return mrdb::Types::VARCHAR;
    default:
      std::cerr << "BPCHAR:" << BPCHAROID << std::endl;
      throw mrdb::SQLException("Posgres OID type:" + std::to_string(oid) + " does not map to any know SQL type.");
  }
}

std::string DatabaseMetaData::typeName(int oid) {
  switch(oid) {
    case INT8OID:         return "int8";
    case BYTEAOID:        return "blob";
    case BITOID:          return "bit";
    case CHAROID:         return "char";
    case DATEOID:         return "date";
    case FLOAT8OID:       return "float8";
    case FLOAT4OID:       return "float4"; // aka mrdb::Types::REAL
    case INT4OID:         return "int";
    case NUMERICOID:      return "numeric"; // aka mrdb::Types::DECIMAL;
    case INT2OID:         return "int2";
    case TIMEOID:         return "time";
    case TIMESTAMPOID:    return "timestamp";
    case BPCHAROID:       return "char";
    case TEXTOID:
    case VARCHAROID:      return "varchar";
    default:
    throw mrdb::SQLException("Posgres OID type:" + std::to_string(oid) + " does not map to any know SQL type.");
  }
}

Types::SQLType DatabaseMetaData::getColumnType(const std::string &tableName, const std::string &columnName) {
  std::string sql = "SELECT atttypid "
                    "FROM pg_attribute JOIN pg_class on attrelid = pg_class.oid "
                    "WHERE relname='"+ tableName +"' AND attname='"+columnName+"'";

  PGresult* result = PQexec( connection_, sql.c_str());
  if(PQresultStatus(result)!=PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);

    throw mrdb::SQLException(errorMessage);
  }

  if(PQntuples(result)!=1) {
    PQclear(result);
    throw mrdb::SQLException("More than one row found describing types for " + tableName + "::" + columnName);
  }

  int oid = std::stoi(PQgetvalue(result, 0, 0));
  PQclear(result);

  return sqlType(oid);
}

mrdb::ResultSet* DatabaseMetaData::getColumns() {
  std::string sql =
    "SELECT c.relname as TABLE_NAME, "
    "       f.attname as COLUMN_NAME, "
    "       pg_catalog.format_type(f.atttypid,f.atttypmod) AS COLUMN_TYPE "
    "FROM pg_catalog.pg_class c "
    "  LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace "
    "  JOIN pg_attribute f ON c.oid = f.attrelid "
    "WHERE c.relkind = 'r' "
    "  AND n.nspname <> 'pg_catalog' "
    "  AND n.nspname <> 'information_schema' "
    "  AND n.nspname !~ '^pg_toast' "
    "  AND pg_catalog.pg_table_is_visible(c.oid) "
    "  AND f.attnum > 0 "
    "ORDER BY c.relname, f.attnum";

  PGresult *result = PQexec(connection_, sql.c_str());
  if(PQresultStatus(result)!=PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);

    throw mrdb::SQLException(errorMessage);
  }


  return new mrdb::postgres::ResultSet(connection_, result);
}

} // namespace mrdb
} // namespace postgres
