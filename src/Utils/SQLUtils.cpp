#include "Utils/SQLUtils.h"
#include <map>
#include <memory>

using namespace std;

namespace minerule {

  SQLUtils::Type
  SQLUtils::getType(odbc::Types::SQLType type) throw(MineruleException) {
    switch( type ) {
    case odbc::Types::BIGINT:
    case odbc::Types::DECIMAL:
    case odbc::Types::DOUBLE:
    case odbc::Types::FLOAT:
    case odbc::Types::INTEGER:
    case odbc::Types::NUMERIC:
    case odbc::Types::REAL:
    case odbc::Types::SMALLINT:
    case odbc::Types::TINYINT:
      return Numeric;
    case odbc::Types::CHAR:
    case odbc::Types::VARCHAR:
    case odbc::Types::LONGVARCHAR:
      return String;
    case odbc::Types::DATE:
    case odbc::Types::TIME:
    case odbc::Types::TIMESTAMP:
      return DateTime;
    case odbc::Types::BINARY:
    case odbc::Types::LONGVARBINARY:
    case odbc::Types::VARBINARY:
      return Binary;
    case odbc::Types::BIT:
      return Bit;
    default: 
      throw MineruleException(MR_ERROR_INTERNAL,"Unexpected kind of SQL data type");
    }
  }

  std::string SQLUtils::quote(const std::string& str) {
    // We have to substitute each "'" with "''" in str (since
    // in sql values cannot contain single "'".
    string rest = str;
    string result="";

    while( rest!="" ) {
      size_t p = rest.find("'");
      if(p==rest.npos) {
	result+=rest;
	rest="";
      } else {
	result+=rest.substr(0,p);
	result+="''";
	rest = rest.substr(p+1,rest.length()-p-1);
      }
    }

    return "'"+result+"'";
  }

  void
  SQLUtils::removeHeadBodyFromAttrName( string& str ) {
    size_t pos;
    if( (pos=str.find("HEAD."))!=str.npos ) {
      str.erase(pos,5);
    }
    if( (pos=str.find("BODY."))!=str.npos ) {
      str.erase(pos,5);
    }
  }


#if 0  
  // It seems that the following code does not work any more
  // A new implementation is given below, unfortunately it is 
  // not as clean and efficient as the one below.
  SQLUtils::Type
  SQLUtils::getType( odbc::Connection* connection,
		     const std::string& tabName, 
		     string colName ) throw (MineruleException) {
    removeHeadBodyFromAttrName(colName);
    
    try {
      odbc::DatabaseMetaData* dbmd = connection->getMetaData();
      auto_ptr<odbc::ResultSet> rs( dbmd->getColumns(
	   "", // mysql does not support catalogs, what about the others?
	   "", // no schema patterns
	   tabName,
	   colName));
      
      if( rs->next() ) 
	return getType( (odbc::Types::SQLType) rs->getShort(5) );
    } catch (odbc::SQLException& e) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR, "Cannot access to the database metadata"
			      "the reason is:" + e.getMessage());
    }
    
    throw MineruleException(MR_ERROR_DATABASE_ERROR, "Cannot access to the database metadata");
  }
#endif
  

  SQLUtils::Type
  SQLUtils::getType( odbc::Connection* connection,
		     const std::string& tabName, 
		     string colName ) throw (MineruleException) {
    removeHeadBodyFromAttrName(colName);
    
    try {
      static map<pair<string,string>, SQLUtils::Type > typeCatalogue;
      map<pair<string,string>, SQLUtils::Type>::const_iterator it =
	typeCatalogue.find(pair<string,string>(tabName,colName));

      if(it!=typeCatalogue.end())
	return it->second;

      // we are now going to load the catalogue
      odbc::DatabaseMetaData* dbmd = connection->getMetaData();
      auto_ptr<odbc::ResultSet> rs( dbmd->getColumns(
	   "", // mysql does not support catalogs, what about the others?
	   "", // no schema patterns
	   tabName,
	   "")); // this is the matter of the problme, the library bugs when this is not ""
      
      while( rs->next() ) {
	typeCatalogue[pair<string,string>(tabName, rs->getString(4))]=
	  getType( (odbc::Types::SQLType) rs->getShort(5) );
      }

      // now we should find the columns in the catalogue!
      it = typeCatalogue.find(pair<string,string>(tabName,colName));
      if(it!=typeCatalogue.end())
	return it->second;
    } catch (odbc::SQLException& e) {
      throw MineruleException(MR_ERROR_DATABASE_ERROR, "Cannot access to the database metadata"
			      "the reason is:" + e.getMessage());
    }
    
    throw MineruleException(MR_ERROR_DATABASE_ERROR, "Cannot access to the database metadata. More precisely, cannot determine type of column ``"+colName+"'' of table ``"+tabName+"''");
  }

} // namespace
