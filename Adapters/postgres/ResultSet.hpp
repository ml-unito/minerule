#ifndef MRDB_POSTGRES_RESULTSET_H_KU2BC3N3I__
#define MRDB_POSTGRES_RESULTSET_H_KU2BC3N3I__

#include <minerule/mrdb/ResultSet.hpp>
#include <libpq-fe.h>
#include <iostream>

namespace mrdb {
namespace postgres {

class ResultSet : public mrdb::ResultSet {
private:
  PGresult *result_;
  ResultSetMetaData* metadata_;
  int numRows_;
  int currentRow_;

public:
  ResultSet(PGresult *result)
      : result_(result), metadata_(NULL), numRows_(PQntuples(result)), currentRow_(-1) {  };

  virtual ~ResultSet();

  virtual bool next();

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
