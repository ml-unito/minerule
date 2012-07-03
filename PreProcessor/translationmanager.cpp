#include "translationmanager.h"

namespace minerule {

string
TranslationManager::getTranslationTableNameForColumn(
			  const string& tableName,
			  const string& columnName) const {
  return string("TMANAGER_")+tableName+"_"+columnName;
}

string
TranslationManager::getTranslatedColumnNameForColumn(
			  const string& tableName, 
			  const string& columnName) const {
  return columnName+"_translated";
}

string
TranslationManager::getOriginalColumnNameForColumn(
                          const string& tableName,
			  const string& columnName) const {
  return columnName;
}

string
TranslationManager::getTranslationTableNameForTable(
			  const string& tableName ) const {
  return string("TMANAGER_"+tableName);
}

} // namespace minerule
