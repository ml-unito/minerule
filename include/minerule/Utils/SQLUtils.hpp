//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __SQLUTILS_H__
#define __SQLUTILS_H__

#include <string>
#include <string.h>
#include "minerule/mrdb/Connection.hpp"
#include "minerule/mrdb/Types.hpp"
#include "minerule/mrdb/ResultSet.hpp"
#include "minerule/mrdb/ResultSetMetaData.hpp"
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Utils/Converter.hpp"


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
      getType( mrdb::Types::SQLType ) throw(MineruleException);

    static Type
      getType( mrdb::ResultSet* rs, int colNum ) {
      return getType( (mrdb::Types::SQLType)
		      rs->getMetaData()->getColumnType(colNum) );
    }

    static Type
      getType( mrdb::Connection* connection,
	       const std::string& tabName,
	       std::string colName ) throw (MineruleException);

    static bool
      isNumericType(mrdb::Types::SQLType type) {
      return getType(type)==Numeric;
    }

    static bool
      isStringType(mrdb::Types::SQLType type) {
      return getType(type)==String;
    }

    static bool
      isBinaryType(mrdb::Types::SQLType type) {
      return getType(type)==Binary;
    }

    static bool
      isDateTimeType(mrdb::Types::SQLType type) {
      return getType(type)==DateTime;
    }

    static bool
      isBitType(mrdb::Types::SQLType type) {
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
