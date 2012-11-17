#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>

#include "Database/SourceRowAttribute.h"
#include "Database/SourceRowColumnIds.h"
#include "Database/SourceRowAttributeCollection.h"

using namespace std;

namespace minerule {

  /**
   * This class models the concept of SourceRow. 
   * It is useful in order to abstract away details about the
   * structure of the source table. 
   */

  class HeadBodySourceRow {
    friend  ostream& operator<<(ostream& os, const HeadBodySourceRow& item);
  private:
    SourceRowElement* groupBody;
    SourceRowElement* clusterBody;
    SourceRowElement* body;
    SourceRowElement* clusterHead;
    SourceRowElement* head;

    static const SourceRowEmptyElement emptyElement;
    
  public:
    // create a new item as a copy of the current row of the result set
    HeadBodySourceRow(odbc::ResultSet* resultSet,
		      const SourceRowColumnIds& srd);

    // copy constructor
    HeadBodySourceRow(const HeadBodySourceRow& item);
		      

    virtual ~HeadBodySourceRow();

    void
      init(odbc::ResultSet* resultSet,
	   const SourceRowColumnIds& srd);


    const SourceRowElement&
      getGroupBody() const {
      if(groupBody==NULL) 
	return emptyElement;
      else
	return *groupBody;
    }

    const SourceRowElement& 
      getClusterBody() const {
      if(clusterBody == NULL)
	return emptyElement;
      else 
	return *clusterBody;
    }

    const SourceRowElement& 
      getBody() const {
      if(body==NULL)
	return emptyElement;
      else
	return *body;
    }

    const SourceRowElement& 
      getClusterHead() const {
      if( clusterHead==NULL ) 
	return emptyElement;
      else
	return *clusterHead;
    }

    const SourceRowElement& 
      getHead() const {
      if(head==NULL)
	return emptyElement;
      else
	return *head;
    }
  };


ostream& operator<<(ostream& os, const HeadBodySourceRow& sr);


}; // END NAMESPACE MINERULE


#endif
