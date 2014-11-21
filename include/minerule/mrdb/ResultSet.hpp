#ifndef RESULT_SET_H_JKEJNCI3L19__
#define RESULT_SET_H_JKEJNCI3L19__

#include <string>
#include "ResultSetMetaData.hpp"

namespace mrdb {
class ResultSet {
public:
  virtual ~ResultSet() {}

  // @return true if the current result set is before the first record.
  virtual bool isBeforeFirst() const = 0;

  // @return true if the current result set is after the last record.
  virtual bool isAfterLast() const = 0;

  // Move the result set to the next available result row. Upon creation the
  // result set is positioned just before the first record, so that a call to
  // this method is necessary *before* accessing to the first row.
  // @return true if the new position is inside the result set
  virtual bool next() = 0;

  // @return the value (assuming it is of type double) at the current row and
  // given column index.
  virtual double getDouble(int columnIndex) = 0;

  // @return the value (assuming it is of type int) at the current row and given
  // column index.
  virtual int getInt(int columnIndex) = 0;

  // @return the value (assuming it is of type long) at the current row and given
  // column index.
  virtual long getLong(int columnIndex) = 0;

  // @return the value (assuming it is of type string) at the current row and
  // given column index.
  virtual std::string getString(int columnIndex) = 0;

  // @return the meta data about this result set
  virtual mrdb::ResultSetMetaData* getMetaData() = 0;
};
}

#endif /* end of include guard: RESULT_SET_H_JKEJNCI3L19__ */
