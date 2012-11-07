#ifndef __TABLETRANSLATIONMANAGERSTANDARDSQL__H__
#define __TABLETRANSLATIONMANAGERSTANDARDSQL__H__

#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>
#include<odbc++/types.h>

#include "translationmanager.h"

/**
 * Provides an implementation of a TranslationManager using only 
 * standard SQL primitives.
 *
 * @author Roberto Esposito (esposito@di.unito.it)
 */

namespace minerule {

class TranslationManagerStandardSQL : public TranslationManager {
  friend class TranslatedTableStandardSQL;
 private:
  // data members
  /** 
   * The connection used to access to the db
   */
  odbc::Connection* connection;  

  /**
   * set to true if every type of column has to be
   * translated, set to 0 if integers and smallint
   * may be left untraslated
   */
  bool translateEverything; 

  // static constants ============================================

  /**
   * The prefix used to distinguish TranslationManager tables from
   * user ones
   */
  static conststd::string TM_PREFIX;
  /**
   * The name of the table in the DB that will hold the mappings
   * between original tables and translated ones
   */
  static conststd::string TABLEDICTIONARY_NAME;

  /**
   * Name of the column of table TABLEDICTIONARY_NAME that holds
   * the orginal table names
   */
  static conststd::string TABLEDICTIONARY_ORIGINAL_TABLE;
  /**
   * Max length of a orignal table name
   */
  static const int TABLEDICTIONARY_ORIGINAL_TABLE_LEN;
  /**
   * Name of the column of table TABLEDICTIONARY_NAME that holds
   * the translated table names
   */
  static conststd::string TABLEDICTIONARY_TRANSLATED_TABLE;
  /**
   * Max length of a translated table name
   */
  static const int TABLEDICTIONARY_TRANSLATED_TABLE_LEN;

  // private methods ==============================================

  /**
   * Checks if table TABLEDICTIONARY_NAME exists and, in case, create
   * it.
   */
  void ensureTableDictionaryExist() const throw(odbc::SQLException);

  /**
   * Checks if the DB contain the table named TABLEDICTIONARY_NAME.
   * Returning true if it does, false otherwise.
   */
  bool tableDictionaryExists() const  throw(odbc::SQLException);

  /**
   * Perform all the actions necessary to translate tableName. If
   * anything goes wrong it should revert the DB to the state it was
   * before the method invocation throwing, afterwards, an
   * SQLException describing the original failure. In case something
   * wrong happens during the operations needed to restore the
   * original state of the DB, the exception will describe the reason
   * of the latter failure.
   */
  void doTranslateTable(const std::string& tableName) 
    const throw(odbc::SQLException);
  

  /**
   * The method checks whether translateEverything is true and behave
   * accordingly. In particular whenever it is not true some politic
   * can be used to decide, accordingly to colType, whether a column
   * has to be translated or not. The method should return true regardless
   * colType whenever translateEverything is true.
   */
  bool shouldTranslateColumnsOfType(int colType) const;
  

  /**
   * Quote thestd::string in input using the sintax prescribed by standard 
   * SQL
   */
  virtualstd::string quote(const std::string&) const; 

  /**
   * returns a valid quoted attribute name. Since it
   * seems there is no standard way in SQL for quoting attribute
   * names, the function simply returns the attribute name. Note that
   * this means that you cannot have column names containing spaces or
   * other special characters.
   */
  virtualstd::string quoteAttribute(const std::string&) const;

  /**
   * Returns the name of the SQL integer type (since dimensions of 
   * integers are not standard, it may be sometime useful to change this
   * value to match some desiderata.
   */
  virtualstd::string getIntTypeName() const {
    return std::string("INTEGER"); 
  }

  void updateTManagerDictionary(const std::string& table) const throw(odbc::SQLException);

  /**
   * Builds the translation table for column named rsmd->getColumnName(colPos).
   * It throw an exception if something weird occurs (most likely the table
   * already exists).
   */

  virtual void createTranslationTableForColumn(
		 const std::string& table,
		 int colPos,
		 odbc::ResultSetMetaData& ) const throw (odbc::SQLException);

  /**
   * Performs all the operations needed to translate the given
   * column. I.e.  It creates the associated translation table (by
   * means of createTranslationTableForColumn()) and popultate it.
   */
  virtual void doTranslateColumn(
		     const std::string& tableName,
		     int colPos,
		     odbc::ResultSetMetaData&
		     ) const throw(odbc::SQLException);

  /**
   * Drop the table containing the translations for the given column.
   */
  virtual void dropTranslationForColumn(
		      const std::string& tableName,
		     int colPos,
		     odbc::ResultSetMetaData&
		     ) const throw(odbc::SQLException);

  /**
   * The method is called by doTranslateColumn when it populates the
   * the translation table for a given row. The method should call
   * the appropriate PreparedStatement setter method specifying column
   * 2 and accordingly to the type of rsmd->getColumnType(1). For instance
   * if rsmd->getColumnType(1)==SQL_INTEGER_TYPE it should call
   * pstm->setInt(2, rs->getInt(1)).
   * Actually the method is implemented as:
   *   pstm->setstring(2, rs->getString(1)) 
   * regardless the actual column type. It should work fine in most
   * cases.
   */

  virtual void doSetResultSetParameter(
		       odbc::PreparedStatement* pstm,
		       odbc::ResultSet* rs,
		       odbc::ResultSetMetaData* rsmd) const;

virtual void buildLists(
	    const std::string& table,
	    odbc::ResultSetMetaData* rsmd,
	    std::string& newTable,
	    std::string& selectList,
	    std::string& tableList,
	    std::string& conditionList ) const throw (odbc::SQLException);


void finalizeTranslationOfTable(
				const std::string& table,
				odbc::ResultSetMetaData* rsmd
				) const throw (odbc::SQLException);


TranslatedTable* 
  newAccessPointForTable(const std::string& tableName) const throw (odbc::SQLException);
  // public methods ==========================================================
 public:
  /**
   * Constructor. The object will use the given connection for each
   * and every access to the DB. Moreover the parameter 
   * translateEverythingOption will be used to determine when care
   * has to be taken in choosing which attribute has to be translated.
   * When this option is false the method shouldTranslateColumnsOfType
   * should decide which columns types has to be translated.
   */
  TranslationManagerStandardSQL(odbc::Connection* aConnection, 
				bool translateEverythingOption=true);

  
  /**
   * Implements TranslationTable::translateTable using only standard
   * SQL primitives.
   */
  TranslatedTable* 
    translateTable(const std::string& tableName) const throw(odbc::SQLException);

  /**
   * Implements TranslationTable::alreadyTranslated using only
   * standard SQL primitives.
   */
  pair<bool,string> 
    alreadyTranslated(const std::string& tableName) const throw(odbc::SQLException);
};

} // namespace minerule

#endif
