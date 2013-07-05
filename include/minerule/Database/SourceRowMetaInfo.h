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
#ifndef __SOURCEROWDESCRIPTOR_H__
#define __SOURCEROWDESCRIPTOR_H__

#include <string>
#include <string.h>
#include <vector>
#include <odbc++/resultset.h>

#include "minerule/Database/SourceRowColumnIds.h"
#include "minerule/Parsers/ParsedMinerule.h"


namespace minerule {

	class SourceRowAttrCollectionDescriptor {
		std::string dataDefinition;
		std::string columnNames;
		int columnsCount;

		void setColumnNames(odbc::ResultSet* rs, const std::vector<int>& collectionElems);

		std::string dataDefinitionForElem(odbc::ResultSet* rs, int elem);

		void setDataDefinition(odbc::ResultSet* rs, const std::vector<int>& collectionElems);

	public:
		SourceRowAttrCollectionDescriptor() {}
		SourceRowAttrCollectionDescriptor( odbc::ResultSet* rs, const std::vector<int>& collectionElems);
		
		void init(odbc::ResultSet* rs, const std::vector<int>& collectionElems );
  
		const std::string& getSQLDataDefinition() const;
		const std::string& getSQLColumnNames() const;
		unsigned int       getColumnsCount() const { return columnsCount; }
		std::string questionMarks() const;
	};

	class SourceRowMetaInfo {
	private:
		SourceRowAttrCollectionDescriptor group;
		SourceRowAttrCollectionDescriptor clusterBody;
		SourceRowAttrCollectionDescriptor body;
		SourceRowAttrCollectionDescriptor clusterHead;
		SourceRowAttrCollectionDescriptor head;
    
	public:
		// Builds a SourceRowMetaInfo from an SourceRowColumnIds and 
		// the source dataset
		SourceRowMetaInfo(odbc::ResultSet* rs, const SourceRowColumnIds& rowDes);
		
		// Builds a SourceRowMetaInfo from a ParsedMinerule
		SourceRowMetaInfo(odbc::Connection*, const ParsedMinerule& minerule);


		const SourceRowAttrCollectionDescriptor& getGroup() const {
			return group;
		}

		const SourceRowAttrCollectionDescriptor& getClusterBody() const {
			return clusterBody;
		}

		const SourceRowAttrCollectionDescriptor& getBody() const {
			return body;
		}

		const SourceRowAttrCollectionDescriptor& getClusterHead() const {
			return clusterHead;
		}

		const SourceRowAttrCollectionDescriptor& getHead() const {
			return head;
		}
	};

}

#endif
