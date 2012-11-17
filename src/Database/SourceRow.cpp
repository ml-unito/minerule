#include "Database/SourceRow.h"
#include "Utils/MineruleLogs.h"

namespace minerule {

  const SourceRowEmptyElement HeadBodySourceRow::emptyElement;

/* ************ */
/* SourceRow methods */
/* ************ */

  HeadBodySourceRow::HeadBodySourceRow(odbc::ResultSet* resultSet,
				       const SourceRowColumnIds& srd) {

    odbc::ResultSetMetaData* rsmd = resultSet->getMetaData();

    groupBody =    SourceRowElement::createElement(rsmd,resultSet,srd.groupBodyElems);
    clusterBody =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterBodyElems);
    body =         SourceRowElement::createElement(rsmd,resultSet,srd.bodyElems);
    clusterHead =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterHeadElems);
    head =         SourceRowElement::createElement(rsmd,resultSet,srd.headElems);	
  }

  HeadBodySourceRow::HeadBodySourceRow(const HeadBodySourceRow& rhs) {
#define CREATE(a) (((rhs.a)!=NULL)?(a)=rhs.a->copy():(a)=NULL)

    CREATE(groupBody);
    CREATE(clusterBody);
    CREATE(body);
    CREATE(clusterHead);
    CREATE(head);

#undef CREATE
  }



HeadBodySourceRow::~HeadBodySourceRow() {
#define DESTROY(a) if((a)!=NULL) delete a;

  DESTROY(groupBody);
  DESTROY(clusterBody);
  DESTROY(body);
  DESTROY(clusterHead);
  DESTROY(head);

#undef DESTROY
}

void 
HeadBodySourceRow::init(odbc::ResultSet* resultSet,
			const SourceRowColumnIds& srd) {

#define DESTROY(a) if((a)!=NULL) delete a;

  DESTROY(groupBody);
  DESTROY(clusterBody);
  DESTROY(body);
  DESTROY(clusterHead);
  DESTROY(head);

  odbc::ResultSetMetaData* rsmd = resultSet->getMetaData();

  groupBody =    SourceRowElement::createElement(rsmd,resultSet,srd.groupBodyElems);
  clusterBody =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterBodyElems);
  body =         SourceRowElement::createElement(rsmd,resultSet,srd.bodyElems);
  clusterHead =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterHeadElems);
  head =         SourceRowElement::createElement(rsmd,resultSet,srd.headElems);

#undef DESTROY
}



/* ***************** *
 * printing routines *
 * ***************** */

std::ostream& 
operator<<(std::ostream& os, const HeadBodySourceRow& sr) {
  os << "groupBody:" << sr.getGroupBody() << std::endl;
  os << " clusterBody:" << sr.getClusterBody() << std::endl;
  os << " body:" << sr.getBody() << std::endl;
  os << " clusterHead:" << sr.getClusterHead() << std::endl;
  os << " head:" << sr.getHead() << std::endl;

  return os;
}  

} // end namespace
