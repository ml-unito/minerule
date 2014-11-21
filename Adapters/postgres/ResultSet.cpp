#include "ResultSet.hpp"
#include "minerule/Utils/Converter.hpp"
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

    double ResultSet::getDouble(int columnIndex) {
      return minerule::Converter(PQgetvalue(result_, currentRow_, columnIndex-1)).toDouble();
    }


    int ResultSet::getInt(int columnIndex) {
      return minerule::Converter(PQgetvalue(result_, currentRow_, columnIndex-1)).toLong();
    }


    std::string ResultSet::getString(int columnIndex) {
      return std::string(PQgetvalue(result_, currentRow_, columnIndex-1));
    }

    mrdb::ResultSetMetaData* ResultSet::getMetaData() {
      return new mrdb::postgres::ResultSetMetaData(result_);
    }

    long ResultSet::getLong(int columnIndex) {
      return minerule::Converter(PQgetvalue(result_, currentRow_, columnIndex-1)).toLong();
    }

    bool ResultSet::isAfterLast() const {
      return currentRow_ >= numRows_;
    }

    bool ResultSet::isBeforeFirst() const {
      return currentRow_ < 0;
    }


  }
}
