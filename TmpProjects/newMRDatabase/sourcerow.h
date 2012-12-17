#ifndef __ITEM_H__
#define __ITEM_H__

#include <vector>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>

#include "minerule/Database/SourceRowAttribute.h"
#include "minerule/Database/SourceRowColumnIds.h"
#include "minerule/Database/SourceRowAttributeCollection.h"

using namespace std;

namespace minerule {

  /**
   * This class models the concept of SourceRow. 
   * It is useful in order to abstract away details about the
   * structure of the source table. 
   */

  class SourceRow {
    friend  ostream& operator<<(ostream& os, const SourceRow& item);
  private:
    SourceRowElement* group;
    SourceRowElement* clusterBody;
    SourceRowElement* body;
    SourceRowElement* clusterHead;
    SourceRowElement* head;

    static const SourceRowEmptyElement emptyElement;
    
  public:
    // create a new item as a copy of the current row of the result set
    SourceRow(odbc::ResultSet* resultSet,
		      const SourceRowColumnIds& srd);

    // copy constructor
    SourceRow(const SourceRow& item);
		      

    virtual ~SourceRow();

    void
      init(odbc::ResultSet* resultSet,
	   const SourceRowColumnIds& srd);


    const SourceRowElement&
      getGroup() const {
      if(group==NULL) 
	return emptyElement;
      else
	return *group;
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


ostream& operator<<(ostream& os, const SourceRow& sr);


}; // END NAMESPACE MINERULE


#endif
