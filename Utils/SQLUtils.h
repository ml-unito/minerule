#ifndef __SQLUTILS_H__
#define __SQLUTILS_H__

#include <string>
#include <string.h>
#include <odbc++/types.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>
#include <odbc++/databasemetadata.h>
#include "Utils/MineruleException.h"
#include "Utils/Converter.h"


namespace minerule {

  class SQLUtils {
  public:
    typedef enum {
      Numeric,
      String,
      Binary,
      DateTime,
      Bit
    } Type;

    static void
    removeHeadBodyFromAttrName( std::string& str );

    static Type 
      getType( odbc::Types::SQLType ) throw(MineruleException);

    static Type
      getType( odbc::ResultSet* rs, int colNum ) {
      return getType( (odbc::Types::SQLType)
		      rs->getMetaData()->getColumnType(colNum) );
    }

    static Type
      getType( odbc::Connection* connection,
	       const std::string& tabName, 
	       std::string colName ) throw (MineruleException);

    static bool
      isNumericType(odbc::Types::SQLType type) {
      return getType(type)==Numeric;
    }

    static bool
      isStringType(odbc::Types::SQLType type) {
      return getType(type)==String;
    }

    static bool
      isBinaryType(odbc::Types::SQLType type) {
      return getType(type)==Binary;
    }

    static bool
      isDateTimeType(odbc::Types::SQLType type) {
      return getType(type)==DateTime;
    }

    static bool
      isBitType(odbc::Types::SQLType type) {
      return getType(type)==Bit;
    }

    static std::string quote(const std::string& str);

    static bool isAttribute(const std::string& str) {
    if(str.length()==0)
      return false;

    if( isdigit(str[0]) )
      return false;

    if( str.find_first_of("\"'")!=str.npos )
      return false;

    if( Converter(str).isNumber() )
      return false;

    return true;
    }

  };

} // namespace minerule


#endif

