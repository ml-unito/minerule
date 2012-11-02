#include "MRDatabase/sourcerow.h"
#include "Utils/MineruleLogs.h"

namespace minerule {

  const SourceRowEmptyElement HeadBodySourceRow::emptyElement;

/* ************ */
/* SourceRow methods */
/* ************ */

  HeadBodySourceRow::HeadBodySourceRow(odbc::ResultSet* resultSet,
				       const HeadBodySourceRowDescription& srd) {

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
			const HeadBodySourceRowDescription& srd) {

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

ostream& 
operator<<(ostream& os, const HeadBodySourceRow& sr) {
  os << "groupBody:" << sr.getGroupBody() << endl;
  os << " clusterBody:" << sr.getClusterBody() << endl;
  os << " body:" << sr.getBody() << endl;
  os << " clusterHead:" << sr.getClusterHead() << endl;
  os << " head:" << sr.getHead() << endl;

  return os;
}  

} // end namespace
