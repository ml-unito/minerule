#include "Optimizer/OptimizerCatalogue.h"
#include "Utils/MineruleOptions.h"

namespace minerule {

void 
QueryResult::Iterator::readElems(int id, ItemSetType& elems) 
  throw (MineruleException, odbc::SQLException, std::exception) {
  ps_elems->setInt(1,id);
  odbc::ResultSet* rs = ps_elems->executeQuery();

  try {
    while( rs->next() ) {
      std::string str = rs->getString(1);
      ItemType elem;
      elem.setSourceRowElement(*SourceRowElement::deserializeElementFromString(str));
      elems.push_back(elem);
    }
  } catch (...) {
    delete rs;
    throw;
  }
  
  delete rs;
}


void
QueryResult::Iterator::init( const std::string& rulesTable,
			   double support,
			   double confidence ) throw( MineruleException, odbc::SQLException ) {
  odbc::Connection* conn =
    MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection();
  state=conn->createStatement();
 std::string query = 
    "SELECT bodyId, headId, supp, con "
    "FROM " + rulesTable + " "
    "WHERE supp>=" + Converter(support).toString() + " AND "
    "con>="+Converter(confidence).toString();

  ps_elems = conn->prepareStatement(
			   "SELECT elem FROM "+rulesTable+"_elems "+
			   "WHERE id=?");

  MRDebug() << "QueryResult::Iterator, the filter query is:" << query << std::endl;

  rs_rules = state->executeQuery(query);
}

bool QueryResult::Iterator::next() {
  return rs_rules->next();
}

void QueryResult::Iterator::getRule( QueryResult::Rule& r ) throw(MineruleException, odbc::SQLException, std::exception) {
  r.body = new ItemSetType();
  r.head = new ItemSetType();
  readElems(  rs_rules->getInt(1), *r.body );
  readElems(  rs_rules->getInt(2), *r.head );
  r.support = rs_rules->getFloat(3);
  r.confidence = rs_rules->getFloat(4);
}

} // namespace
