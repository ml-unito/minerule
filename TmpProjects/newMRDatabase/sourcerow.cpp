#include "sourcerow.h"


namespace minerule {

  const SourceRowEmptyElement SourceRow::emptyElement;

/* ************ */
/* SourceRow methods */
/* ************ */

  SourceRow::SourceRow(odbc::ResultSet* resultSet,
				       const SourceRowColumnIds& srd) {

    odbc::ResultSetMetaData* rsmd = resultSet->getMetaData();

    groupBody =    SourceRowElement::createElement(rsmd,resultSet,srd.groupBodyElems);
    clusterBody =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterBodyElems);
    body =         SourceRowElement::createElement(rsmd,resultSet,srd.bodyElems);
    clusterHead =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterHeadElems);
    head =         SourceRowElement::createElement(rsmd,resultSet,srd.headElems);
  }

  SourceRow::SourceRow(const SourceRow& rhs) {
#define CREATE(a) (((rhs.a)!=NULL)?(a)=rhs.a->copy():(a)=NULL)

    CREATE(groupBody);
    CREATE(clusterBody);
    CREATE(body);
    CREATE(clusterHead);
    CREATE(head);

#undef CREATE
  }



SourceRow::~SourceRow() {
#define DESTROY(a) if((a)!=NULL) delete a;

  DESTROY(groupBody);
  DESTROY(clusterBody);
  DESTROY(body);
  DESTROY(clusterHead);
  DESTROY(head);

#undef DESTROY
}

void 
SourceRow::init(odbc::ResultSet* resultSet,
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

  /*
void
printAttrCollection(ostream& os, const SourceRowAttributeCollection& attrColl) {
  if(attrColl.getConstAttributes().size()==0) {
    os << "(attr_collection_empty)";
    return;
  }

  SourceRowAttributeCollection::CollectionType::const_iterator it;
  it = attrColl.getConstAttributes().begin();
  os << "attr_collection:(";

  os << **it;
  it++;
  
  while(it!=attrColl.getConstAttributes().end()) {
    os << "," << **it;
    it++;
  }

  os <<")";
}

ostream& operator<<(ostream& os, const SourceRowAttributeCollection& attrColl) {
  printAttrCollection(os, attrColl);
  return os;
}
  */


ostream& 
operator<<(ostream& os, const SourceRow& sr) {
  os << "groupBody:" << sr.getGroupBody() << endl;
  os << " clusterBody:" << sr.getClusterBody() << endl;
  os << " body:" << sr.getBody() << endl;
  os << " clusterHead:" << sr.getClusterHead() << endl;
  os << " head:" << sr.getHead() << endl;

  return os;
}  

} // end namespace
