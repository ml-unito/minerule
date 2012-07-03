#include "sourcerowdescriptor.h"
#include "sourcerowcommon.h"
#include <odbc++/resultsetmetadata.h>
#include <iostream>
#include <string.h>


using namespace minerule;
using namespace std;


/* **********************************
 * SourceRowDescriptor methods
 * **********************************/

void
SourceRowAttrCollectionDescriptor::
setColumnNames(odbc::ResultSet* rs,
	       const std::vector<int>& collectionElems) {
  vector<int>::const_iterator it = collectionElems.begin();
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
SourceRowAttrCollectionDescriptor::
dataDefinitionForElem(odbc::ResultSet* rs, int elem) {
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

  //  std::cerr << "!!!" << getCodedColumnName(elem) + " " +
  //    std::string(rsmd->getColumnTypeName(elem)) + " " + 
  //    std::string(buf);
      
  if( rsmd->getColumnType(elem) == odbc::Types::DATE )
    strcpy(buf,"");
  
  return 
    getCodedColumnName(elem) + " " +
    std::string(rsmd->getColumnTypeName(elem)) + " " + 
    std::string(buf);
}

void
SourceRowAttrCollectionDescriptor::
setDataDefinition(odbc::ResultSet* rs,
		  const vector<int>& collectionElems) {
  vector<int>::const_iterator it = collectionElems.begin();
  dataDefinition = "";

  if(it!=collectionElems.end()) {
    dataDefinition += dataDefinitionForElem(rs, *it);
    it++;
  }

  for(; it!=collectionElems.end(); it++ ) {
    dataDefinition += "," + dataDefinitionForElem(rs,*it);
  }
}


SourceRowAttrCollectionDescriptor::
SourceRowAttrCollectionDescriptor(odbc::ResultSet* rs,
				  const vector<int>& collectionElems) {
  setColumnNames(rs,collectionElems);
  setDataDefinition(rs,collectionElems);
  columnsCount = collectionElems.size();
}

const std::string&
SourceRowAttrCollectionDescriptor::
getSQLDataDefinition() const {
  return dataDefinition;
}

const std::string&
SourceRowAttrCollectionDescriptor::
getSQLColumnNames() const {
  return columnNames;
}


/* **********************************
 * SourceRowDescriptor methods
 * **********************************/

SourceRowDescriptor::
SourceRowDescriptor(odbc::ResultSet* rs, 
		    const HeadBodySourceRowDescription& rowDes) :
  groupBody(rs,rowDes.groupBodyElems),
  clusterBody(rs,rowDes.clusterBodyElems),
  body(rs,rowDes.bodyElems),
  clusterHead(rs,rowDes.clusterHeadElems),
  head(rs,rowDes.headElems) {

}
