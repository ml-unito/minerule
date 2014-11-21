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
#include <iostream>
#include <string.h>

#include "minerule/Database/SourceRowMetaInfo.hpp"
#include "minerule/Database/SourceRowColumnIds.hpp"
#include "minerule/mrdb/Types.hpp"


using namespace minerule;



/* **********************************
 * SourceRowMetaInfo methods
 * **********************************/

void
SourceRowAttrCollectionDescriptor::
setColumnNames(mrdb::ResultSet* rs, const std::vector<int>& collectionElems) {
  mrdb::ResultSetMetaData* rsmd = rs->getMetaData();

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
SourceRowAttrCollectionDescriptor::dataDefinitionForElem(mrdb::ResultSet* rs, int elem) {
  mrdb::ResultSetMetaData* rsmd = rs->getMetaData();

  int precision = rsmd->getPrecision(elem);
  int scale = rsmd->getScale(elem);

  char buf[255];
  if( precision == 0 ) {
	  strcpy(buf,"");
  } else {
    if(scale==0)
      sprintf(buf,"(%d)",precision);
    else
      sprintf(buf,"(%d,%d)", precision,scale);
  }

  if( rsmd->getColumnType(elem) == mrdb::Types::DATE )
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
SourceRowAttrCollectionDescriptor::setDataDefinition(mrdb::ResultSet* rs, const std::vector<int>& collectionElems) {
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

void SourceRowAttrCollectionDescriptor::init(mrdb::ResultSet* rs, const std::vector<int>& collectionElems ) {
    setColumnNames(rs,collectionElems);
    setDataDefinition(rs,collectionElems);
    columnsCount = collectionElems.size();
}



SourceRowAttrCollectionDescriptor::SourceRowAttrCollectionDescriptor(mrdb::ResultSet* rs, const std::vector<int>& collectionElems) {
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

SourceRowMetaInfo::SourceRowMetaInfo(mrdb::ResultSet* rs, const SourceRowColumnIds& rowDes)
	 : group(rs,rowDes.groupElems),
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

 SourceRowMetaInfo::SourceRowMetaInfo(mrdb::Connection* odbc_connection, const ParsedMinerule& minerule) {
	 ParsedMinerule::AttrVector attr_list;
	 attr_list.insert(attr_list.end(), minerule.ga.begin(), minerule.ga.end());
	 attr_list.insert(attr_list.end(), minerule.ca.begin(), minerule.ca.end());
     attr_list.insert(attr_list.end(), minerule.ba.begin(), minerule.ba.end());
     attr_list.insert(attr_list.end(), minerule.ca.begin(), minerule.ca.end());
	 attr_list.insert(attr_list.end(), minerule.ha.begin(), minerule.ha.end());

	 std::string query =
		 "SELECT " + AttributesUtil::names_to_string(attr_list) +
		 " FROM " + minerule.tab_source + " LIMIT 1";

	 mrdb::Statement* state = odbc_connection->createStatement();
	 mrdb::ResultSet* rs = state->executeQuery(query);

	 AttributesUtil attrUtils;
	 group.init(rs, attrUtils.generatePositions(minerule.ga) );
	 clusterBody.init(rs, attrUtils.generatePositions(minerule.ca) );
	 body.init(rs, attrUtils.generatePositions(minerule.ba));
	 clusterHead.init(rs, attrUtils.generatePositions(minerule.ca));
	 head.init(rs,attrUtils.generatePositions(minerule.ha));

	 delete rs;
	 delete state;
 }
