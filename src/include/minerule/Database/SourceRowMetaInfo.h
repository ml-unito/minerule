#ifndef __SOURCEROWDESCRIPTOR_H__
#define __SOURCEROWDESCRIPTOR_H__

#include <string>
#include <string.h>
#include <vector>
#include <odbc++/resultset.h>

#include "Database/SourceRowColumnIds.h"
#include "Parsers/ParsedMinerule.h"


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
		SourceRowAttrCollectionDescriptor groupBody;
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


		const SourceRowAttrCollectionDescriptor& getGroupBody() const {
			return groupBody;
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
