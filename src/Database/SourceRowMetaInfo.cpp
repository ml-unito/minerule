#include "Database/SourceRowMetaInfo.h"
#include "Database/SourceRowColumnIds.h"
#include <odbc++/resultsetmetadata.h>
#include <iostream>
#include <string.h>


using namespace minerule;



/* **********************************
 * SourceRowMetaInfo methods
 * **********************************/

void
SourceRowAttrCollectionDescriptor::
setColumnNames(odbc::ResultSet* rs, const std::vector<int>& collectionElems) {
  odbc::ResultSetMetaData* rsmd = rs->getMetaData();
  
  std::vector<int>::const_iterator it = collectionElems.begin();
  columnNames = "";

  if(it!=collectionElems.end()) {
    columnNames += rsmd->getColumnName(*it);
    it++;
  }

  for(; it!=collectionElems.end(); it++ ) {
    columnNames += "," + rsmd->getColumnName(*it);
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
    rsmd->getColumnName(elem) + " " +
    std::string(rsmd->getColumnTypeName(elem)) + " " + 
    std::string(buf);
}

std::string SourceRowAttrCollectionDescriptor::questionMarks() const {
	std::string result;
	for( size_t i=0; i<columnsCount; ++i) {
		if( i!=0 ) 
			result += ",?";
		else
			result += "?";
	}
	
	return result;
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
 * SourceRowMetaInfo methods
 * **********************************/

SourceRowMetaInfo::SourceRowMetaInfo(odbc::ResultSet* rs, const SourceRowColumnIds& rowDes) 
	 : groupBody(rs,rowDes.groupBodyElems),
	   clusterBody(rs,rowDes.clusterBodyElems),
	   body(rs,rowDes.bodyElems),
	   clusterHead(rs,rowDes.clusterHeadElems),
	   head(rs,rowDes.headElems) { }
 
 
 class AttributesUtil {	 
	 int curr_attr_pos;
 public:
	 AttributesUtil() : curr_attr_pos(1) {}
	 
	 std::vector<int> generatePositions(const ParsedMinerule::AttrVector& attrs) {
		 std::vector<int> result;
		 for( ParsedMinerule::AttrVector::const_iterator it = attrs.begin(); it!=attrs.end(); ++it ) {
			 result.push_back(curr_attr_pos++);
		 }
		 return result;
	 }
	 
	 static std::string names_to_string(const ParsedMinerule::AttrVector& attrs) {
		 std::string result;
		 for( ParsedMinerule::AttrVector::const_iterator it = attrs.begin(); it!=attrs.end(); ++it ) {
			 if( it != attrs.begin() ) result += ",";
			 result += *it;
		 }
		 
		 return result;
	 }
 };
 
 SourceRowMetaInfo::SourceRowMetaInfo(odbc::Connection* odbc_connection, const ParsedMinerule& minerule) {
	 ParsedMinerule::AttrVector attr_list;
	 attr_list.insert(attr_list.end(), minerule.ga.begin(), minerule.ga.end());
	 attr_list.insert(attr_list.end(), minerule.ca.begin(), minerule.ca.end()); 
     attr_list.insert(attr_list.end(), minerule.ba.begin(), minerule.ba.end()); 
     attr_list.insert(attr_list.end(), minerule.ca.begin(), minerule.ca.end());
	 attr_list.insert(attr_list.end(), minerule.ha.begin(), minerule.ha.end());
	 
	 std::string query = 
		 "SELECT " + AttributesUtil::names_to_string(attr_list) +
		 " FROM " + minerule.tab_source + " LIMIT 1";
				 
	 odbc::Statement* state = odbc_connection->createStatement();
	 odbc::ResultSet* rs = state->executeQuery(query);
	 
	 AttributesUtil attrUtils;
	 groupBody.init(rs, attrUtils.generatePositions(minerule.ga) );
	 clusterBody.init(rs, attrUtils.generatePositions(minerule.ca) );
	 body.init(rs, attrUtils.generatePositions(minerule.ba));
	 clusterHead.init(rs, attrUtils.generatePositions(minerule.ca));
	 head.init(rs,attrUtils.generatePositions(minerule.ha));
	 
	 delete rs;
	 delete state;
 }
