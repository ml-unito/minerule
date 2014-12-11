#include <assert.h>

#include "PreparedStatement.hpp"
#include "ResultSet.hpp"
#include <string.h>

namespace mrdb {
namespace postgres {

PreparedStatement::PreparedStatement(PGconn *conn, std::string sql) : connection_(conn) {
  static int stmtCount = 0;

  std::pair<int, std::string> formattedSql = formatParams(sql);
  numParams_ = formattedSql.first;
  statementName_ = "MRDB_PREPARED_STATEMENT_" + std::to_string(stmtCount++);

  initParams();

  PGresult* cmdResult = PQprepare(connection_, statementName_.c_str(), formattedSql.second.c_str(), 0, NULL);
  if (PQresultStatus(cmdResult) != PGRES_COMMAND_OK) {
    std::string errorMessage = PQresultErrorMessage(cmdResult);
    PQclear(cmdResult);

    throw mrdb::SQLException(errorMessage);
  }

  PQclear(cmdResult);
}


bool PreparedStatement::execute() {
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
PreparedStatement::executeQuery() {
  PGresult *result = PQexecPrepared(connection_, statementName_.c_str(), numParams_, params_, NULL, NULL, 0);

  if (PQresultStatus(result) != PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);
    throw mrdb::SQLException(errorMessage);
  }

  return new mrdb::postgres::ResultSet(result);
}


#define SETTER_DEFINITION \
  setParam(idx-1, std::to_string(val).c_str() )

void PreparedStatement::setDouble(int idx, double val) {
  SETTER_DEFINITION;
}

void PreparedStatement::setInt(int idx, int val) {
  SETTER_DEFINITION;
}

void PreparedStatement::setString(int idx, const std::string &val) {
  setParam(idx-1, val.c_str());
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
        std::to_string(idx) + " not valid!"
        " Valid range is [0," + std::to_string(numParams_-1) + "]";

    throw mrdb::SQLException(errorMessage);
  }

  if(params_[idx] != NULL) {
    free(params_[idx]);
    params_[idx] = NULL;
  }

  params_[idx] = strndup(val.c_str(), 255);
  assert( params_[idx] != NULL);
}

std::pair<int, std::string>
PreparedStatement::formatParams(std::string sql) const {
  int paramNumber = 0;
  int questionMarkPos;

  while ((questionMarkPos = sql.find("?")) != std::string::npos) {
    std::string placeHolder =
    std::string("$") + std::to_string(++paramNumber);
    sql.replace(questionMarkPos, 1, placeHolder);
  }

  return std::pair<int, std::string>(paramNumber, sql);
}



} /* postgres */
} /* mrdb */
