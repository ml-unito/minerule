#include "ResultSet.hpp"
#include "ResultSetMetaData.hpp"

namespace mrdb {
  namespace postgres {

    ResultSet::~ResultSet() {
      if(metadata_!=NULL) {
        delete metadata_;
      }

      PQclear(result_);
    }

    bool ResultSet::next() {
      return ++currentRow_ < numRows_;
    }

    bool ResultSet::getBoolean(int columnIndex) {
      return PQgetvalue(result_, currentRow_, columnIndex-1)[0] == 't';
    }

    double ResultSet::getDouble(int columnIndex) {
      return std::stod(PQgetvalue(result_, currentRow_, columnIndex-1));
    }


    int ResultSet::getInt(int columnIndex) {
      return std::stol(PQgetvalue(result_, currentRow_, columnIndex-1));
    }


    std::string ResultSet::getString(int columnIndex) {
      return std::string(PQgetvalue(result_, currentRow_, columnIndex-1));
    }

    mrdb::ResultSetMetaData* ResultSet::getMetaData() {
      return new mrdb::postgres::ResultSetMetaData(connection_, result_);
    }

    long ResultSet::getLong(int columnIndex) {
      return std::stol(PQgetvalue(result_, currentRow_, columnIndex-1));
    }

    bool ResultSet::isAfterLast() const {
      return currentRow_ >= numRows_;
    }

    bool ResultSet::isBeforeFirst() const {
      return currentRow_ < 0;
    }


  }
}
