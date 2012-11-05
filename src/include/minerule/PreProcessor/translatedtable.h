#ifndef __TRANSLATEDTABLE_H__
#define __TRANSLATEDTABLE_H__


using namespace std;

namespace minerule {

class TranslationManager;

/**
 * Provides methods to access to translated tables.
 */


class TranslatedTable {
public:
  /**
   * @return the name of the translated table. The returned value 
   * can be used to form queries involving the translated values. 
   */
  virtual string getTranslatedName() const throw (odbc::SQLException) = 0;

  /**
   * @return the name of the original table. 
   */
virtual  string getOriginalName() const  throw (odbc::SQLException) = 0;

  /**
   * Takes a column name of the current table and returns its translated name.
   * The returned value can be used to build queries on the translated tables.
   *
   * @param columnName the name of a table column
   * @return the name of the translated column
   */
virtual  string getTranslatedColumnName( const string& columnName) const  throw (odbc::SQLException) = 0; 
  
  /* Cannot see any use for this method... if needed I will add it in future
   * @param translatedColumnName a column name of the translated table
   * @return the name, in the original table, of the column named
   * translatedColumnName in the translated table.
   */
// virtual  string getOriginalColumnName( const string& translatedColumnName ) const = 0;

  /**
   * @param columnName a column name of the original table
   * @param value the value to be translated
   * @return the translatedValue
   */
virtual  string getTranslatedValue( const string& columnName,
			     const string& value) const  throw (odbc::SQLException) = 0;
  
  /**
   * @param columnName a column name of the original table
   * @param translatedValue a translated value
   * @return the value of translatedValue in the original table
   */
virtual  string getOriginalValue( const string& columnName,
			   const string& translatedValue) const  throw (odbc::SQLException) = 0;
};

}

#endif
