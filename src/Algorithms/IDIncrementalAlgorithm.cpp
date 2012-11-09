#include <memory>
#include <iterator>
#include "Algorithms/IDIncrementalAlgorithm.h"
#include "Utils/MineruleOptions.h"
#include "Utils/SQLUtils.h"
#include "Database/Connection.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"



namespace minerule {

void
IDIncrementalAlgorithm::getItemInfos( std::string& itemDescr, 
				      HeadBodySourceRowDescription& hbsr ) const {
  assert(attrList!=NULL);
  ParsedMinerule::ListType::const_iterator it;
  int n;
  for(it=attrList->begin(), n=1; it!=attrList->end(); it++,n++) {
    if(itemDescr!="")
      itemDescr+=",";

    itemDescr+=*it;
    hbsr.bodyElems.push_back(n);
  }
}


std::set<ItemType>*
IDIncrementalAlgorithm::fillValidItems(const std::string& constraints) const throw(odbc::SQLException) {
  if(constraints=="")
    return NULL;

  std::set<ItemType>* result = new std::set<ItemType>;

#ifdef MRUSERWARNING
#warning In the future we will assume to have a normalized database. \
  Then we will need to make the following selection over the correct \
  relation of the db instead of over the current source table (which \
  is needingly larger).
#endif 
 std::string itemDescr;
  HeadBodySourceRowDescription hbsr;
  
  getItemInfos(itemDescr,hbsr);

 std::string query =
    "SELECT DISTINCT "+itemDescr+" "+
    "FROM "+minerule->getParsedMinerule().tab_source+" "+
    (constraints.size()>0 ?
    "WHERE "+constraints : "");
  
  odbc::Connection* con = 
    MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection();

  std::auto_ptr<odbc::Statement> state(con->createStatement());
  std::auto_ptr<odbc::ResultSet> rs(state->executeQuery(query.c_str()));

  while(rs->next()) {
    HeadBodySourceRow sr(rs.get(), hbsr);
    result->insert(sr.getBody());
  };

  return result;
}


/**
 * Notice that we implement our checkInclusion mechanism instead of
 * using STL ``includes'' function. There is a performance reason to do
 * that: the STL includes function, does not make assumption on the
 * internal representation of the two sets, i.e. it does not exploit
 * the fact that one can search one element in one of the sets in
 * O(log(N)) time, moreover we also know that one of the two sets is
 * *much* smaller than the other one. The total complexity of the STL
 * algorithm is O(N+M) where N and M are the sizes of the two
 * sets. This version is O(N*log(M)), however since M is usually much
 * bigger than N this strategy should pay in our case (e.g., a tipical
 * N size may something as 3, while M is usually in the order of 1000,
 * the linear algorithm takes about 1000 operations, while the one
 * implemented here takes 3*log(1000) ~= 30 operations.
 */

bool
IDIncrementalAlgorithm::checkInclusion(const std::set<ItemType>& validOnes, 
				       const ItemSetType& foundOnes) const {
  ItemSetType::const_iterator it;
  for(it=foundOnes.begin();
      it!=foundOnes.end();
      it++) {
    if(validOnes.find(*it)==validOnes.end())
      return false;
  }

  return true;
}

bool
IDIncrementalAlgorithm::checkInValidRules(const ValidRules& validRules,
					  QueryResult::Rule& r) const {
  assert(r.body!=NULL && r.head!=NULL );
  ValidRules::const_iterator it = validRules.begin();
  bool found = false;
  while(it!=validRules.end() && !found) {
    found=
      (it->first==NULL || checkInclusion(*it->first, *r.body)) &&
      (it->second==NULL|| checkInclusion(*it->second, *r.head));

    it++;
  }

  return found;
}
void
IDIncrementalAlgorithm::filterQueries(const ValidRules& validRules)
  throw(MineruleException,odbc::SQLException) {
  OptimizerCatalogue& cat = 
    MineruleOptions::getSharedOptions().getOptimizations().getCatalogue();
 std::string resName = cat.getResultsetName(
       		minerule->getOptimizationInfo().minerule.tab_result );

  QueryResult::Iterator qri;
  qri.init(resName, 
	   minerule->getOptimizationInfo().minerule.sup, 
	   minerule->getOptimizationInfo().minerule.conf);

  MRLog() << "tab results:" 
	  << resName << std::endl;
  MRLog() << "Orig supp:" 
	  << minerule->getOptimizationInfo().minerule.sup << std::endl;
  MRLog() << "Orig conf:" 
	  << minerule->getOptimizationInfo().minerule.conf << std::endl;

  MRLog() << "Connection ..."<<std::endl;
  Connection connection;
  connection.setOutTableName(minerule->getParsedMinerule().tab_result);
  //  MRLog() << "ok connection.setOutTableName" << std::endl;
 
  connection.setBodyCardinalities(minerule->getParsedMinerule().bodyCardinalities);
  //MRLog() << "ok connection.setBodyCardinalities" << std::endl;

  connection.setHeadCardinalities(minerule->getParsedMinerule().headCardinalities);
  //MRLog() << "ok connection.setHeadCardinalities" << std::endl;

  connection.useODBCConnection( MineruleOptions::getSharedOptions().getOdbc_db().getODBCConnection());
  //MRLog() << "ok connection.useODBCConnection" << std::endl; 

  connection.createResultTables(SourceRowDescriptor(connection.getODBCConnection(), minerule->getParsedMinerule()));
  //MRLog() << "ok connection.createResultTables" << std::endl;

  size_t rcount = 0;
  while(qri.next()) {
    QueryResult::Rule r;
    qri.getRule(r);
    if(checkInValidRules(validRules, r))
      connection.insert(*r.body, *r.head, r.support, r.confidence);

    rcount++;
  }
  MRLog() << "Processed #" << rcount << " rules."<<std::endl;
  //  std::cout << "count: " << rcount << std::endl;
}


void
IDIncrementalAlgorithm::printValidRules(const IDIncrementalAlgorithm::ValidRules& vr) const {
  ValidRules::const_iterator it;
  for(it=vr.begin(); it!=vr.end(); it++) {
    std::cout << "BODIES" << std::endl;
    if(it->first==NULL)
      std::cout << "NULL" << std::endl;
    else
      copy( it->first->begin(),
	    it->first->end(),
	    std::ostream_iterator<ItemType>(cout, "\n") );

    std::cout << "HEADS" << std::endl;
    if(it->second==NULL)
      std::cout << "NULL" << std::endl;
    else
      copy( it->second->begin(),
	    it->second->end(),
	    std::ostream_iterator<ItemType>(cout, "\n") );

  }
}

void 
IDIncrementalAlgorithm::execute() throw(MineruleException,odbc::SQLException) {
  assert(minerule->getParsedMinerule().mc!=NULL);

#ifdef SAVE_TIMINGS_ON_FILE
#warning Temporary code...
    MineruleOptions& opts = MineruleOptions::getSharedOptions();
    ofstream outfile("/home/minerule/mrweb/timings.txt", ios_base::out | ios_base::app);
    assert(outfile);
    outfile << minerule->getParsedMinerule().mc->l_and->sp->val2 << " ";
// temporary code ends
#endif

  MRLogPush("This is IDIncrementalAlgorithm. Starting...");

  ValidRules validRules;
  list_OR_node* it;
  MRLogPush("Building the list of items satisfying the constraints");
  size_t conjNum=0;
  for(it=minerule->getParsedMinerule().mc; it!=NULL; it=it->next) {
    MRLog() << "Conjunct number:" << ++conjNum << std::endl;
   std::string bodyConstraints;
   std::string headConstraints;
    HeadBodyPredicatesSeparator::separate(it->l_and, bodyConstraints, headConstraints);
    // a little bit ugly, but useful. The fillValidItems will use the attrList
    // in order to select the correct portion of the database needed to build
    // the items in the head/body part of the rule. We need to set it to the
    // correct list before calling fillValidItems
    attrList = &minerule->getParsedMinerule().ba;
    std::set<ItemType>* validBodies = fillValidItems( bodyConstraints );
    attrList = &minerule->getParsedMinerule().ha;
    std::set<ItemType>* validHeads  = fillValidItems( headConstraints );
    attrList = NULL;
    validRules.push_back(ValidRule(validBodies,validHeads));
  }

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< " ";
//end
#endif 

  MRLogPop();

  MRLogPush("Filtering past results...");
  filterQueries(validRules);

#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary    
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< " ";
//end
#endif

  MRLogPop();

  //trashing the trashable
  MRLogPush("Cleaning up...");
  ValidRules::const_iterator rule_it;
  for(rule_it=validRules.begin();
      rule_it!=validRules.end();
      rule_it++) {
    if(rule_it->first!=NULL)
      delete rule_it->first;

    if(rule_it->second!=NULL)
      delete rule_it->second;
  }
  MRLogPop();
  
#ifdef SAVE_TIMINGS_ON_FILE
#warning temporary     
outfile << opts.getLogStreamObj().getLogger().getCurrentTimeDelta()<< std::endl;
//end
#endif
  MRLogPop();
}


} // namespace
