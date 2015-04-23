#ifndef MRDB_POSTGRES_RESULTSET_H_KU2BC3N3I__
#define MRDB_POSTGRES_RESULTSET_H_KU2BC3N3I__

#include <minerule/mrdb/ResultSet.hpp>
#include <libpq-fe.h>
#include <iostream>

namespace mrdb {
namespace postgres {

class ResultSet : public mrdb::ResultSet {
private:
  PGconn* connection_; // weak reference
  PGresult *result_;
  ResultSetMetaData* metadata_;
  int numRows_;
  int currentRow_;

public:
  ResultSet(PGconn* connection, PGresult *result)
      : connection_(connection), result_(result), metadata_(NULL), numRows_(PQntuples(result)), currentRow_(-1) {  };

  virtual ~ResultSet();

  virtual bool next();

  virtual bool getBoolean(int columnIndex);
  virtual double getDouble(int columnIndex);
  virtual int getInt(int columnIndex);
  virtual long getLong(int columnIndex);
  virtual std::string getString(int columnIndex);

  virtual mrdb::ResultSetMetaData* getMetaData();

  virtual bool isAfterLast() const;
  virtual bool isBeforeFirst() const;
};
} //namespace mrdb
} //namespace postgres

#endif /* end of include guard: MRDB_POSTGRES_RESULTSET_H_KU2BC3N3I__ */
