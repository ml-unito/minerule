#include "Statement.hpp"
#include "ResultSet.hpp"

namespace mrdb {
namespace postgres {

mrdb::ResultSet *Statement::executeQuery(const std::string &sql)  {
  PGresult *result = PQexec(connection_, sql.c_str());

  if (PQresultStatus(result) != PGRES_TUPLES_OK) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);

    throw mrdb::SQLException(errorMessage);
  }

  return new mrdb::postgres::ResultSet(result);
}

bool Statement::execute(const std::string &sql)  {
  PGresult *result = PQexec(connection_, sql.c_str());
  ExecStatusType status = PQresultStatus(result);
  std::string errorMessage;

  if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK ) {
    std::string errorMessage = PQresultErrorMessage(result);
    PQclear(result);
    throw mrdb::SQLException(errorMessage);
  }

  PQclear(result);
  return status == PGRES_TUPLES_OK;
}

}
}
