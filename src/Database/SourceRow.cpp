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
#include "minerule/Database/SourceRow.hpp"
#include "minerule/Utils/MineruleLogs.hpp"

namespace minerule {

  const SourceRowEmptyElement SourceRow::emptyElement;

/* ************ */
/* SourceRow methods */
/* ************ */

  SourceRow::SourceRow(mrdb::ResultSet* resultSet,
				       const SourceRowColumnIds& srd) {
    mrdb::ResultSetMetaData* rsmd = resultSet->getMetaData();

    group =    SourceRowElement::createElement(rsmd,resultSet,srd.groupElems);
    clusterBody =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterBodyElems);
    body =         SourceRowElement::createElement(rsmd,resultSet,srd.bodyElems);
    clusterHead =  SourceRowElement::createElement(rsmd,resultSet,srd.clusterHeadElems);
    head =         SourceRowElement::createElement(rsmd,resultSet,srd.headElems);	
  }

  SourceRow::SourceRow(const SourceRow& rhs) {
#define CREATE(a) (((rhs.a)!=NULL)?(a)=rhs.a->copy():(a)=NULL)

    CREATE(group);
    CREATE(clusterBody);
    CREATE(body);
    CREATE(clusterHead);
    CREATE(head);

#undef CREATE
  }



SourceRow::~SourceRow() {
#define DESTROY(a) if((a)!=NULL) delete a;

  DESTROY(group);
  DESTROY(clusterBody);
  DESTROY(body);
  DESTROY(clusterHead);
  DESTROY(head);

#undef DESTROY
}

void 
SourceRow::init(mrdb::ResultSet* resultSet,
			const SourceRowColumnIds& srd) {

#define DESTROY(a) if((a)!=NULL) delete a;

  DESTROY(group);
  DESTROY(clusterBody);
  DESTROY(body);
  DESTROY(clusterHead);
  DESTROY(head);

  mrdb::ResultSetMetaData* rsmd = resultSet->getMetaData();

  group =    SourceRowElement::createElement(rsmd,resultSet,srd.groupElems);
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
operator<<(std::ostream& os, const SourceRow& sr) {
  os << "group:" << sr.getGroup() << std::endl;
  os << " clusterBody:" << sr.getClusterBody() << std::endl;
  os << " body:" << sr.getBody() << std::endl;
  os << " clusterHead:" << sr.getClusterHead() << std::endl;
  os << " head:" << sr.getHead() << std::endl;

  return os;
}  

} // end namespace
