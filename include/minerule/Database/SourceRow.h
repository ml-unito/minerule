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
#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>

#include "minerule/Database/SourceRowAttribute.h"
#include "minerule/Database/SourceRowColumnIds.h"
#include "minerule/Database/SourceRowAttributeCollection.h"



namespace minerule {

  /**
	* This class models the concept of SourceRow. 
	* It is useful in order to abstract away details about the
	* structure of the source table. 
   */

	class SourceRow {
		friend  std::ostream& operator<<(std::ostream& os, const SourceRow& item);
	private:
		SourceRowElement* group;
		SourceRowElement* clusterBody;
		SourceRowElement* body;
		SourceRowElement* clusterHead;
		SourceRowElement* head;

		/* The element that is returned when the value cannot be retrieved (e.g., when it is null)*/
		static const SourceRowEmptyElement emptyElement;

	public:
		SourceRow() : group(NULL), clusterBody(NULL), body(NULL), clusterHead(NULL), head(NULL) { }
    // create a new item as a copy of the current row of the result set
		SourceRow(odbc::ResultSet* resultSet, const SourceRowColumnIds& srd);

    // copy constructor
		SourceRow(const SourceRow& item);
		      

		virtual ~SourceRow();

		void init(odbc::ResultSet* resultSet,const SourceRowColumnIds& srd);

		#define ELEM_OR_EMPTY(elem) if((elem)==NULL) return emptyElement; else return *elem;
		
		// --- GETTERS ---
		const SourceRowElement&
		getGroup() const {
			ELEM_OR_EMPTY(group)
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


	std::ostream& operator<<(std::ostream& os, const SourceRow& sr);


}; // END NAMESPACE MINERULE


#endif
