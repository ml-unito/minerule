#include <assert.h>
#include <minerule/Utils/Converter.hpp>

#include "PreparedStatement.hpp"
#include "ResultSet.hpp"

namespace mrdb {
namespace postgres {

PreparedStatement::PreparedStatement(PGconn *conn, std::string sql) throw(mrdb::SQLException &)
    : connection_(conn) {
  static int stmtCount = 0;

  std::pair<int, std::string> formattedSql = formatParams(sql);
  numParams_ = formattedSql.first;
  statementName_ = "MRDB_PREPARED_STATEMENT_" + minerule::Converter(stmtCount++).toString();

  initParams();

  PGresult* cmdResult = PQprepare(connection_, statementName_.c_str(), formattedSql.second.c_str(), 0, NULL);
  if (PQresultStatus(cmdResult) != PGRES_COMMAND_OK) {
    std::string errorMessage = PQresultErrorMessage(cmdResult);
    PQclear(cmdResult);

    throw mrdb::SQLException(errorMessage);
  }

  PQclear(cmdResult);
}


bool PreparedStatement::execute() throw(mrdb::SQLException &) {
  PGresult *result = PQexecPrepared(connection_, statementName_.c_str(), numParams_, params_, NULL, NULL, 0);

  int status = PQresultStatus(result);
  if ( status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);
    throw mrdb::SQLException(errorMessage);
  }

  PQclear(result);
  return status == PGRES_TUPLES_OK;
}

mrdb::ResultSet *
PreparedStatement::executeQuery() throw(mrdb::SQLException &) {
  PGresult *result = PQexecPrepared(connection_, statementName_.c_str(), numParams_, params_, NULL, NULL, 0);

  if (PQresultStatus(result) != PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);
    throw mrdb::SQLException(errorMessage);
  }

  return new mrdb::postgres::ResultSet(result);
}


#define SETTER_DEFINITION \
  setParam(idx-1, minerule::Converter(val).toString().c_str() )

void PreparedStatement::setDouble(int idx, double val) {
  SETTER_DEFINITION;
}

void PreparedStatement::setInt(int idx, int val) {
  SETTER_DEFINITION;
}

void PreparedStatement::setString(int idx, const std::string &val) {
  SETTER_DEFINITION;
}

void PreparedStatement::setLong(int idx, long val) {
  SETTER_DEFINITION;
}


// ----------------------
// PRIVATE MEMBERS
// ----------------------

void PreparedStatement::initParams() {
  params_ = (char**) malloc( sizeof(char*) * numParams_ );
  if(params_==NULL) {
    throw mrdb::SQLException("Out of memory");
  }

  for(int i=0; i<numParams_;++i) {
    params_[i] = NULL;
  }
}

void PreparedStatement::freeParams() {
  for(int i=0; i<numParams_; ++i) {
    if(params_[i] != NULL) {
      free(params_[i]);
    }
  }

  free(params_);
}

void PreparedStatement::setParam(int idx, const std::string& val) {
  if(idx < 0 || idx >= numParams_) {
    std::string errorMessage = std::string("Column index ") +
        minerule::Converter(idx).toString() + " not valid!"
        " Valid range is [0," + minerule::Converter(numParams_-1).toString() + "]";

    throw mrdb::SQLException(errorMessage);
  }

  if(params_[idx] != NULL) {
    free(params_[idx]);
    params_[idx] = NULL;
  }

  params_[idx] = strndup(minerule::Converter(val).toString().c_str(), 255);
  assert( params_[idx] != NULL);
}

std::pair<int, std::string>
PreparedStatement::formatParams(std::string sql) const {
  int paramNumber = 0;
  int questionMarkPos;

  while ((questionMarkPos = sql.find("?")) != std::string::npos) {
    std::string placeHolder =
    std::string("$") + minerule::Converter(++paramNumber).toString();
    sql.replace(questionMarkPos, 1, placeHolder);
  }

  return std::pair<int, std::string>(paramNumber, sql);
}



} /* postgres */
} /* mrdb */
