#include <memory>
#include <iterator>
#include "minerule/Algorithms/IDIncrementalAlgorithm.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Utils/SQLUtils.h"
#include "minerule/Database/Connection.h"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.h"



namespace minerule {

	void
	IDIncrementalAlgorithm::getItemInfos( std::string& itemDescr, SourceRowColumnIds& hbsr ) const {
		assert(attrList!=NULL);
		ParsedMinerule::AttrVector::const_iterator it;
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
		relation of the db instead of aover the current source table (which \
		is needingly larger).
#endif 
		std::string itemDescr;
		SourceRowColumnIds hbsr;
  
		getItemInfos(itemDescr,hbsr);

		std::string query =
			"SELECT DISTINCT "+itemDescr+" "+
			"FROM "+minerule->getParsedMinerule().tab_source+" "+
			(constraints.size()>0 ?	"WHERE "+constraints : "");
  
		odbc::Connection* con = 
			MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::auto_ptr<odbc::Statement> state(con->createStatement());
		std::auto_ptr<odbc::ResultSet> rs(state->executeQuery(query.c_str()));

		while(rs->next()) {
			SourceRow sr(rs.get(), hbsr);
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

bool IDIncrementalAlgorithm::checkInclusion(const std::set<ItemType>& validOnes, const ItemSet& foundOnes) const {
		ItemSet::const_iterator it;
		for(it=foundOnes.begin();it!=foundOnes.end();it++) {
			if(validOnes.find(*it)==validOnes.end())
				return false;
		}

		return true;
	}

	bool
	IDIncrementalAlgorithm::checkInValidRules(const ValidRules& validRules, Rule& r) const {
		ValidRules::const_iterator it = validRules.begin();
		bool found = false;
		while(it!=validRules.end() && !found) {
			found=
				(it->first==NULL || checkInclusion(*it->first, r.getBody())) &&
					(it->second==NULL|| checkInclusion(*it->second, r.getHead()));

			it++;
		}

		return found;
	}
	
	
	void IDIncrementalAlgorithm::filterQueries(const ValidRules& validRules) throw(MineruleException,odbc::SQLException) {
		OptimizerCatalogue& cat = MineruleOptions::getSharedOptions().getOptimizations().getCatalogue();
		
		std::string resName = cat.getResultsetName(minerule->getOptimizationInfo().minerule.tab_result );

		QueryResult::Iterator qri;
		qri.init(resName, minerule->getOptimizationInfo().minerule.sup, minerule->getOptimizationInfo().minerule.conf);

		MRLog() << "tab results:" << resName << std::endl;
		MRLog() << "Orig supp:"   << minerule->getOptimizationInfo().minerule.sup << std::endl;
		MRLog() << "Orig conf:"   << minerule->getOptimizationInfo().minerule.conf << std::endl;

		MRLog() << "Connection ..."<<std::endl;
		Connection connection;
		connection.setOutTableName(minerule->getParsedMinerule().tab_result);
		connection.setBodyCardinalities(minerule->getParsedMinerule().bodyCardinalities);
		connection.setHeadCardinalities(minerule->getParsedMinerule().headCardinalities);
		connection.useODBCConnection( MineruleOptions::getSharedOptions().getODBC().getODBCConnection());
		connection.createResultTables(SourceRowMetaInfo(connection.getODBCConnection(), minerule->getParsedMinerule()));

		size_t rcount = 0;
		while(qri.next()) {
			Rule r;
			qri.getRule(r);
			if(checkInValidRules(validRules, r))
				connection.insert(r.getBody(), r.getHead(), r.getSupport(), r.getConfidence());

			rcount++;
		}
		MRLog() << "Processed #" << rcount << " rules."<<std::endl;
	}


	void 
	IDIncrementalAlgorithm::execute() throw(MineruleException,odbc::SQLException) {
		assert(minerule->getParsedMinerule().mc!=NULL);

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

		MRLogPop(); // Building the list of items satisfying the constraints

		MRLogPush("Filtering past results...");		
		filterQueries(validRules);
		MRLogPop(); // Filtering past results...

  //trashing the trashable
		MRLogPush("Cleaning up...");
		ValidRules::const_iterator rule_it;
		for(rule_it=validRules.begin();rule_it!=validRules.end();rule_it++) {
			if(rule_it->first!=NULL)
				delete rule_it->first;

			if(rule_it->second!=NULL)
				delete rule_it->second;
		}
		MRLogPop(); // Cleaning up...
  

		MRLogPop();  // This is IDIncrementalAlgorithm. Starting...
	}


} // namespace
