#include "Database/SourceRowDescriptor.h"
#include "Database/SourceRowCommon.h"
#include <odbc++/resultsetmetadata.h>
#include <iostream>
#include <string.h>


using namespace minerule;



/* **********************************
 * SourceRowDescriptor methods
 * **********************************/

void
SourceRowAttrCollectionDescriptor::
setColumnNames(odbc::ResultSet* rs, const std::vector<int>& collectionElems) {
  std::vector<int>::const_iterator it = collectionElems.begin();
  columnNames = "";

  if(it!=collectionElems.end()) {
    columnNames += getCodedColumnName(*it);
    it++;
  }

  for(; it!=collectionElems.end(); it++ ) {
    columnNames += "," + getCodedColumnName(*it);
  }
}

std::string
SourceRowAttrCollectionDescriptor::dataDefinitionForElem(odbc::ResultSet* rs, int elem) {
  odbc::ResultSetMetaData* rsmd = rs->getMetaData();

  int precision = rsmd->getPrecision(elem);
  int scale = rsmd->getScale(elem);
  
  char buf[255];
  if( precision == 0 )
    strcpy(buf,"");
  else {
    if(scale==0)
      sprintf(buf,"(%d)",precision);
    else
      sprintf(buf,"(%d,%d)", precision,scale);
  }
      
  if( rsmd->getColumnType(elem) == odbc::Types::DATE )
    strcpy(buf,"");
  
  return 
    getCodedColumnName(elem) + " " +
    std::string(rsmd->getColumnTypeName(elem)) + " " + 
    std::string(buf);
}

void
SourceRowAttrCollectionDescriptor::setDataDefinition(odbc::ResultSet* rs, const std::vector<int>& collectionElems) {
  std::vector<int>::const_iterator it = collectionElems.begin();
  dataDefinition = "";

  if(it!=collectionElems.end()) {
    dataDefinition += dataDefinitionForElem(rs, *it);
    it++;
  }

  for(; it!=collectionElems.end(); it++ ) {
    dataDefinition += "," + dataDefinitionForElem(rs,*it);
  }
}

void SourceRowAttrCollectionDescriptor::init(odbc::ResultSet* rs, const std::vector<int>& collectionElems ) {
    setColumnNames(rs,collectionElems);
    setDataDefinition(rs,collectionElems);
    columnsCount = collectionElems.size();	
}



SourceRowAttrCollectionDescriptor::SourceRowAttrCollectionDescriptor(odbc::ResultSet* rs, const std::vector<int>& collectionElems) {
	init(rs, collectionElems);
}

const std::string&
SourceRowAttrCollectionDescriptor::getSQLDataDefinition() const {
  return dataDefinition;
}

const std::string&
SourceRowAttrCollectionDescriptor::getSQLColumnNames() const {
  return columnNames;
}


/* **********************************
 * SourceRowDescriptor methods
 * **********************************/

SourceRowDescriptor::SourceRowDescriptor(odbc::ResultSet* rs, const HeadBodySourceRowDescription& rowDes) 
	 : groupBody(rs,rowDes.groupBodyElems),
	   clusterBody(rs,rowDes.clusterBodyElems),
	   body(rs,rowDes.bodyElems),
	   clusterHead(rs,rowDes.clusterHeadElems),
	   head(rs,rowDes.headElems) { }
 
 
 class AttributesUtil {	 
	 int curr_attr_pos;
 public:
	 AttributesUtil() : curr_attr_pos(0) {}
	 
	 std::vector<int> generatePositions(const ListType& attrs) {
		 std::vector<int> result;
		 for( ListType::const_iterator it = attrs.begin(); it!=attrs.end(); ++it ) {
			 result.push_back(curr_attr_pos++);
		 }
		 return result;
	 }
	 
	 static std::string names_to_string(const ListType& attrs) {
		 std::string result;
		 for( ListType::const_iterator it = attrs.begin(); it!=attrs.end(); ++it ) {
			 if( *it != attrs.begin()) result += ",";
			 result += *it;
		 }
		 
		 return result;
	 }
 };
 
 SourceRowDescriptor::SourceRowDescriptor(odbc::Connection* odbc_connection, const ParsedMinerule& minerule) {
	 ListType attr_list  = minerule.ga minerule.ca + minerule.ba + minerule.ca + minerule.ha; 
	 std::string query = 
		 "SELECT " + AttributesUtil::names_to_string(attr_list) +
		 " FROM " + minerule.tab_source + " LIMIT 1";
				 
	 odbc::Statement* state = odbc_connection->createStatement();
	 odbc::ResultSet* rs = state->execute(query);
	 
	 AttributesUtil positions;
	 groupBody.init(rs, position.generatePositions(minerule.ga) );
	 clusterBody.init(rs, position.generatePositions(minerule.ca) );
	 body.init(rs, position.generatePositions(ba));
	 clusterHead.init(rs, positions.generatePositions(ca));
	 head.init(rs.init(rs,positions.generatePositions(ha));
	 
	 delete rs;
	 delete state;
 }
