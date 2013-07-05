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
#ifndef __TRANSLATEDTABLESTANDARDSQL_H__
#define __TRANSLATEDTABLESTANDARDSQL_H_


/**
 * Provides methods to access to translated tables generated by 
 * TranslationManagerStandardSQL.
 */

#include "minerule/Utils/common.h"
#include "translatedtable.h"
#include <ext/hash_map>



using namespace __gnu_cxx;

namespace minerule {

class TranslationManagerStandardSQL;

// This class is needed by the hash_map used in TranslatedTableStandardSQL
class Scmp {
public:
  int operator()(const char* s1, const char* s2) const {
    return std::strcmp(s1,s2)==0;
  }
};

class TranslatedTableStandardSQL : public TranslatedTable {
  friend class TranslationManagerStandardSQL;

 private:
  const TranslationManagerStandardSQL* tmanager;
 std::string originalTable;
 std::string translatedTable;
  bool translateEverything;
  typedef hash_map<
    const char*,
    bool, 
    hash<const char*>,
    Scmp> ColumnDictionary;
  
  ColumnDictionary columnsToTranslate ;

 protected:
  void
    setOriginalTableName(const std::string& table) {
    originalTable = table;
  }
  
  void
    setTranslatedTableName(const std::string& table) {
    translatedTable = table;
  }

 /**
   * This method should be called only in the table constructor, it
   * update the translateEverything table in order to reflect the
   * actual configuration of manager->translateEverything and the
   * table structure.
   */

  virtual void updateColumnsToTranslate();

 public:
  TranslatedTableStandardSQL(const TranslationManagerStandardSQL* manager, 
			     const std::string& parOriginalTable,
			     const std::string& parTranslatedTable,
			     bool parTranslateEverything) :
    tmanager(manager), originalTable(parOriginalTable), translatedTable(parTranslatedTable) {
    translateEverything = parTranslateEverything;
    
    updateColumnsToTranslate();
  }

 

  /**
   * @return the name of the translated table. The returned value 
   * can be used to form queries involving the translated values. 
   */
  virtualstd::string getTranslatedName() const throw (odbc::SQLException);

  /**
   * @return the name of the original table. 
   */
  virtualstd::string getOriginalName() const throw (odbc::SQLException);

  /**
   * Takes a column name of the current table and returns its translated name.
   * The returned value can be used to build queries on the translated tables.
   *
   * @param columnName the name of a table column
   * @return the name of the translated column
   */
  virtualstd::string getTranslatedColumnName( const std::string& columnName) const throw (odbc::SQLException); 
  
  /* I sincerely cannot see any applicatio for this method... if needed
   * I will add it to the interface.
   *
   * @param translatedColumnName a column name of the translated table
   * @return the name, in the original table, of the column named
   * translatedColumnName in the translated table.
   */
  //std::string getOriginalColumnName( const std::string& translatedColumnName ) const = 0;

  /**
   * @param columnName a column name of the original table
   * @param value the value to be translated
   * @return the translatedValue
   */
  virtualstd::string getTranslatedValue( const std::string& columnName,
			     const std::string& value) const throw (odbc::SQLException);
  
  /**
   * @param columnName a column name of the original table
   * @param translatedValue a translated value
   * @return the value of translatedValue in the original table
   */
virtual std::string getOriginalValue( const std::string& columnName,
			   const std::string& translatedValue) const throw (odbc::SQLException);


 virtual ~TranslatedTableStandardSQL(); 
};
}

#endif
