#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>

#include "Database/SourceRowAttribute.h"
#include "Database/SourceRowCommon.h"
#include "Database/SourceRowAttributeCollection.h"



namespace minerule {

  /**
	* This class models the concept of SourceRow. 
	* It is useful in order to abstract away details about the
	* structure of the source table. 
   */

	class HeadBodySourceRow {
		friend  std::ostream& operator<<(std::ostream& os, const HeadBodySourceRow& item);
	private:
		SourceRowElement* groupBody;
		SourceRowElement* clusterBody;
		SourceRowElement* body;
		SourceRowElement* clusterHead;
		SourceRowElement* head;

		/* The element that is returned when the value cannot be retrieved (e.g., when it is null)*/
		static const SourceRowEmptyElement emptyElement;
    
	public:
    // create a new item as a copy of the current row of the result set
		HeadBodySourceRow(odbc::ResultSet* resultSet,
			const HeadBodySourceRowDescription& srd);

    // copy constructor
		HeadBodySourceRow(const HeadBodySourceRow& item);
		      

		virtual ~HeadBodySourceRow();

		void init(odbc::ResultSet* resultSet,const HeadBodySourceRowDescription& srd);

		#define ELEM_OR_EMPTY(elem) if((elem)==NULL) return emptyElement; else return *elem;
		
		// --- GETTERS ---
		const SourceRowElement&
		getGroupBody() const {
			ELEM_OR_EMPTY(groupBody)
		}

		const SourceRowElement& 
		getClusterBody() const {
			ELEM_OR_EMPTY(clusterBody);
		}

		const SourceRowElement& 
		getBody() const {
			ELEM_OR_EMPTY(body);
		}

		const SourceRowElement& 
		getClusterHead() const {
			ELEM_OR_EMPTY(clusterHead);
		}

		const SourceRowElement& 
		getHead() const {
			ELEM_OR_EMPTY(head);
		}
	};


	std::ostream& operator<<(std::ostream& os, const HeadBodySourceRow& sr);


}; // END NAMESPACE MINERULE


#endif
