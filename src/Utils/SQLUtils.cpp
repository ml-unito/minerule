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

#include <map>
#include <memory>

#include "minerule/Utils/SQLUtils.hpp"
#include "minerule/Utils/MineruleLogs.hpp"
#include "minerule/mrdb/Connection.hpp"
#include "minerule/mrdb/DatabaseMetaData.hpp"

namespace minerule {

SQLUtils::Type
SQLUtils::getType(mrdb::Types::SQLType type) throw(MineruleException) {
  switch (type) {
  case mrdb::Types::BIGINT:
  case mrdb::Types::DECIMAL:
  case mrdb::Types::DOUBLE:
  case mrdb::Types::FLOAT:
  case mrdb::Types::INTEGER:
  case mrdb::Types::NUMERIC:
  case mrdb::Types::REAL:
  case mrdb::Types::SMALLINT:
  case mrdb::Types::TINYINT:
    return Numeric;
  case mrdb::Types::CHAR:
  case mrdb::Types::VARCHAR:
  case mrdb::Types::LONGVARCHAR:
    return String;
  case mrdb::Types::DATE:
  case mrdb::Types::TIME:
  case mrdb::Types::TIMESTAMP:
    return DateTime;
  case mrdb::Types::BINARY:
  case mrdb::Types::LONGVARBINARY:
  case mrdb::Types::VARBINARY:
    return Binary;
  case mrdb::Types::BIT:
    return Bit;
  default:
    throw MineruleException(MR_ERROR_INTERNAL,
                            "Unexpected kind of SQL data type:" +
                                Converter(type).toString());
  }
}

std::string SQLUtils::quote(const std::string &str) {
  // We have to substitute each "'" with "''" in str (since
  // in sql values cannot contain single "'".
  std::string rest = str;
  std::string result = "";

  while (rest != "") {
    size_t p = rest.find("'");
    if (p == rest.npos) {
      result += rest;
      rest = "";
    } else {
      result += rest.substr(0, p);
      result += "''";
      rest = rest.substr(p + 1, rest.length() - p - 1);
    }
  }

  return "'" + result + "'";
}

void SQLUtils::removeHeadBodyFromAttrName(std::string &str) {
  size_t pos;
  if ((pos = str.find("HEAD.")) != str.npos) {
    str.erase(pos, 5);
  }
  if ((pos = str.find("BODY.")) != str.npos) {
    str.erase(pos, 5);
  }
}

// It seems that the following code does not work any more
// A new implementation is given below, unfortunately it is
// not as clean and efficient as the one below.
SQLUtils::Type SQLUtils::getType(mrdb::Connection *connection,
                                 const std::string &tabName,
                                 std::string colName) throw(MineruleException) {
  removeHeadBodyFromAttrName(colName);

  try {
    mrdb::DatabaseMetaData *dbmd = connection->getMetaData();

    return getType(dbmd->getColumnType(tabName, colName));
  } catch (mrdb::SQLException &e) {
    throw MineruleException(MR_ERROR_DATABASE_ERROR,
                            std::string("Cannot access to the database metadata the reason is:") +
                                e.what());
  }

  throw MineruleException(MR_ERROR_DATABASE_ERROR,
                          "Cannot access to the database metadata");
}

// SQLUtils::Type SQLUtils::getType(mrdb::Connection *connection,
//                                  const std::string &tabName,
//                                  std::string colName)
//                                  throw(MineruleException) {
//   removeHeadBodyFromAttrName(colName);
//
//   try {
//     typedef std::pair<std::string, std::string> TabColName;
//     typedef std::map<TabColName, SQLUtils::Type> TypeCatalogue;
//     static TypeCatalogue typeCatalogue;
//
//     TypeCatalogue::const_iterator it;
//     TabColName tabColName(tabName, colName);
//     it = typeCatalogue.find(tabColName);
//
//     if (it != typeCatalogue.end())
//       return it->second;
//
//     MRDebug("Checking mrdb type for column:" + colName + " of table " +
//             tabName);
//     // we are now going to load the catalogue
//     mrdb::DatabaseMetaData *dbmd = connection->getMetaData();
//     std::auto_ptr<mrdb::ResultSet> rs(dbmd->getColumns(
//         "", // mysql does not support catalogs, what about the others?
//         "", // no schema patterns
//         tabName,
//         "")); // this is the matter of the problem, the library bugs when
//         this
//               // is not ""
//
//     while (rs->next()) {
//       typeCatalogue[TabColName(tabName, rs->getString(4))] =
//           getType((mrdb::Types::SQLType)rs->getShort(5));
//     }
//
//     // now we should find the columns in the catalogue!
//     it = typeCatalogue.find(tabColName);
//
//     if (it != typeCatalogue.end())
//       return it->second;
//
//   } catch (mrdb::SQLException &e) {
//     throw MineruleException(MR_ERROR_DATABASE_ERROR,
//                             "Cannot access to the database metadata"
//                             "the reason is:" +
//                                 e.what());
//   }
//
//   throw MineruleException(MR_ERROR_DATABASE_ERROR,
//                           "Cannot access to the database metadata. More "
//                           "precisely, cannot determine type of column ``" +
//                               colName + "'' of table ``" + tabName + "''");
// }

} // namespace
