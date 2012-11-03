#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<list>
#include<algorithm>

/*
  #ifndef String
  #  include<wxstrgnu.h>
  typedef wxString String;
  #endif
*/

#include"Optimizer/OptimizedMinerule.h"
#include"Parsers/ParsedMinerule.h"

#include<odbc++/drivermanager.h>
#include<odbc++/connection.h>
#include<odbc++/resultset.h>
#include<odbc++/statement.h>
#include<odbc++/resultsetmetadata.h>

#include "Utils/MineruleOptions.h"
#include "Optimizer/QueryNormalizer.h"
#include "PredicateUtils/PredicateUtils.h"

using namespace std;
using namespace odbc;

namespace minerule {

  void OptimizedMinerule::attributesInPredicate( const list_OR_node* l, set<std::string>& result ) {
  	while( l!=NULL ) {
  		list_AND_node* l_and = l->l_and;
  		while( l_and!=NULL ) {
  			if( !Converter(l_and->sp->val1).isNumber() )
  				result.insert(l_and->sp->val1);
    
  			if( !Converter(l_and->sp->val2).isNumber() )
  				result.insert(l_and->sp->val2);
    			
			l_and=l_and->next;
  		}
       
  		l=l->next;
  	}
  }

  bool OptimizedMinerule::predicateAttributesAreIncluded( const ParsedMinerule& mr1,
							  const ParsedMinerule& mr2 ) {
    set<string> mr1Attrs;
    set<string> mr2Attrs;

    attributesInPredicate( mr1.mc, mr1Attrs );
    attributesInPredicate( mr2.mc, mr2Attrs );

    return includes(mr2Attrs.begin(), mr2Attrs.end(), mr1Attrs.begin(), mr1Attrs.end());

  }

  bool OptimizedMinerule::isACandidateQuery( const ParsedMinerule& mr1, //candidate
					     					 const ParsedMinerule& mr2  ) { // target 
    return mr1.ga==mr2.ga &&
      mr1.ca==mr2.ca &&
      mr1.ra==mr2.ra &&
      mr1.ba==mr2.ba &&
      mr1.ha==mr2.ha &&
      mr1.c_aggr_list == mr2.c_aggr_list &&
      mr1.sup<=mr2.sup &&
      mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
      mr1.headCardinalities.contains(mr2.headCardinalities) && 
      mr1.tab_source==mr2.tab_source &&
      // above are the standard checking 
      // if in addition both the queries are item dependent (mr1 is
      // item dependent by hypothesis).
      mr1.hasIDConstraints() &&
      // and the all the attributes on which mr1.mc predicates are included
      // in those on which mr2 predicates
      predicateAttributesAreIncluded(mr1,mr2);
    // then mr1 is a candidate query
  }

  void OptimizedMinerule::checkForCombinedQueries(  ) {
    MRLogPush("Searching for equivalences due to query combinations...");
    vector<Predicate> candidates;
    
    vector<ParsedMinerule>::const_iterator it;

    // disabling the logging of the Parser since it would confuse
    // the user.
    MineruleOptions::getSharedOptions().getLogStreamObj().disable();

    ostream& log = MRLog() << "Candidates are: ";
    
    for(it=optInfo.minerulesToCombine.begin(); it!=optInfo.minerulesToCombine.end(); it++) {
      if(it!=optInfo.minerulesToCombine.begin()) log<<", ";

      log << it->tab_result;
      candidates.push_back(Predicate(it->mc));
    }
    log << endl;

    // re-enabling the log 
    MineruleOptions::getSharedOptions().getLogStreamObj().enable();

    Predicate target(minerule.mc);
    
	// FIXME: The third parameters ("Sales") seems to be obsoleted and not used any more
    GAQueryCombinator qCombinator(target, candidates, "Sales");
    MineruleOptions::Optimizations::Combinator& combopt =
      MineruleOptions::getSharedOptions().getOptimizations().getCombinator();
	
    qCombinator.setMaxDisjuncts(combopt.getMaxDisjuncts());
    qCombinator.setMaxQueries(combopt.getMaxQueries());
    qCombinator.setMaxDistinctPredicates(combopt.getMaxDistinctPredicates());
    qCombinator.setTimeOutThreshold(combopt.getTimeOutThreshold());

    try {
      qCombinator.evolve();
    } catch( TimeOutException& e ) {
      MRLog() << "The algorithm timed out." << endl;
    }
    

	// FIXME: all of the following is about output. It should be moved into an appropriate method
    if( qCombinator.getResult().empty() ) {
      MRLog() << "No valid combination found!" << endl;
    } else { 
      std::string qryString;

      GAQueryCombinator::QueryOrList::const_iterator itOr;
      for(itOr=qCombinator.getResult().begin(); itOr!=qCombinator.getResult().end();  itOr++) {
		if(itOr!=qCombinator.getResult().begin())
		  qryString += " OR ";
      
		GAQueryCombinator::QueryAndList::const_iterator itAnd;
		for(itAnd=itOr->begin(); itAnd!=itOr->end(); itAnd++) {
		  if( itAnd!=itOr->begin() )
		    qryString += " AND ";

		  qryString += optInfo.minerulesToCombine[*itAnd].tab_result;
		}
      }
    

      MRLog() 	<< "A combination of queries equivalent to the given one"
	      		<< " has been found. The found query follows:" << endl;
      MRLog() 	<< qryString << endl;

      optInfo.combinationFormula = qCombinator.getResult();
      optInfo.relationship = Combination;
    }
 
    MRLogPop();
  }


  void OptimizedMinerule::optimize()
  {
#ifdef MRUSERWARNING
#warning minerule qui sotto viene inizializzata piu'' volte senza motivo. \
 converrebbe eliminarla del tutto e usare dappertutto optInfo.minerule.
#endif

    if(minerule.miningTask!=MTMineRules) {
      MRLog() << "Skipping optimization, " << miningTaskToString(minerule.miningTask) << " task is still unsupported by the optimizer" << endl;
      MRDebug() << "Skipping optimization, " << miningTaskToString(minerule.miningTask) << " task is still unsupported by the optimizer" << endl;
      return;
    }

    list<ParsedMinerule> lPreviousMr;
    if(!MineruleOptions::getSharedOptions().getOptimizations().getTryOptimizations())
      return;

    bool avoidCombinationDetection = 
      MineruleOptions::getSharedOptions().getOptimizations().getAvoidCombinationDetection() 
      || !minerule.hasIDConstraints();

    OptimizerCatalogue& catalogue = 
      MineruleOptions::getSharedOptions().getOptimizations().getCatalogue();

    QueryNormalizer normalizer( catalogue, minerule );
    normalizer.normalize();
    optInfo.minerule = minerule;
    optInfo.relationship = None;
    MRDebug() << "Minerule text after normalization:" << minerule.getText() << endl;

    odbc::Connection* connection = 
      MineruleOptions::getSharedOptions().getOdbc_db().getConnection();

    odbc::Statement* statement = NULL;
    ResultSet* rs = NULL;

    //lettura delle minerules precedenti
    try
      {
	MRLogPush("Optimization phase..");
	statement = connection->createStatement();
	ResultSet* rs = statement->executeQuery("SELECT query_text,query_name FROM mr_query;");

	MRLog() << "Reading past minerules" << endl;	
	while(rs->next()) {
	  ParsedMinerule mr;
	  mr.init(rs->getString(1).c_str());
	  MRDebug() << "Optimize: trying minerule:" << mr.getText() << endl;

	  if( !avoidCombinationDetection && isACandidateQuery(mr,minerule) ) {
	    optInfo.minerulesToCombine.push_back(mr);
	  }

	  MineruleRelationship currentRel = 
	    getMineruleRelationship(mr,minerule);

	  if( currentRel==Equivalence &&
	      MineruleOptions::getSharedOptions().getOptimizations().getAvoidEquivalenceDetection() )
	    currentRel=Dominance;

	  switch( currentRel ) {
	  case Equivalence:
	    MRLog() << "A past minerule found which includes the current one!" << endl;
	    MRLog() << "Name of the found minerule:" << rs->getString(2) << endl;
	    MRLog() << "(The algorithm will store this information in the catalogue" << endl;
	    MRLog() << " and exit)" << endl;
	    optInfo.relationship = Equivalence;
	    optInfo.minerule = mr;
	    break;
	  case Dominance:
	    {
	      MRLog() << "Minerule named ``" << mr.tab_result 
		      <<"'' dominates the current one." <<endl;
	      if(optInfo.relationship==None) {
		// since relationship==None, there is not yet any information
		// about other Dominances. We simply store the found minerule
		optInfo.minerule = mr;
		MRLog() << "Minerule " << "``" << mr.tab_result <<"'' is now the best promising" 
			<< " dominant minerule." << endl;
	      } else {
		// we must keep the current minerule only if its result set
		// is smaller than the one of the past Dominant query found
		CatalogueInfo newQryInfo;
		CatalogueInfo oldQryInfo;
		OptimizerCatalogue::getMRQueryInfo( mr.tab_result, newQryInfo );
		OptimizerCatalogue::getMRQueryInfo( optInfo.minerule.tab_result, oldQryInfo );
		if( newQryInfo.resSize<oldQryInfo.resSize) {
		  optInfo.minerule = mr;
		  MRLog() << "Minerule " << "``" << mr.tab_result 
			  <<"'' is now the best promising" 
			  << " dominant minerule." << endl;
		} else {
		  MRLog() << "Keeping the previously found dominant minerule" << endl;
		}
	      }
	      
	      optInfo.relationship = Dominance;
	    }
	    break;
	  default:
	    // nothing to do
	    break;
	  };

	  // if the found relationship is Equivalence, then we cannot do better
	  // and exit immediately from the loop, otherwise we check if we can
	  // find some minerule which is a better candidate for the incremental
	  // algorithms.
	  if(optInfo.relationship==Equivalence) 
	    break;
	}

	if(optInfo.relationship==None) 
	  MRLog() << "No interesting past minerules found." << endl;

	if(optInfo.relationship==Dominance) 
	  MRLog() << "The best promising dominant minerule found is ``" 
		  << optInfo.minerule.tab_result << "''" << endl;
	
	MRLogPop();
      } catch (odbc::SQLException& e) {
	MRErr() << e.what() << endl;
	if(rs!=NULL)        delete rs;
	if(statement!=NULL) delete statement;
	throw;
      }

    if(optInfo.relationship==None && 
       !avoidCombinationDetection) {
      checkForCombinedQueries();
      if(optInfo.relationship==None)
	optInfo.minerulesToCombine.clear();
    }
    
    delete statement;
    delete rs;
  }




  bool OptimizedMinerule::firstMineruleIncludesSecondMinerule(
				 const ParsedMinerule& mr1,
				 const ParsedMinerule& mr2) {
#ifdef MRUSERWARNING
#warning Bisognerebbe controllare anche body==head, ma in questo momento body e head sono "fusi" in ra.
#endif

    
    Predicate mr1pmc(mr1.mc);
    Predicate mr2pmc(mr2.mc);
    Predicate mr1pgc(mr1.gc);
    Predicate mr2pgc(mr2.gc);
    Predicate mr1pcc(mr1.cc);
    Predicate mr2pcc(mr2.cc);

    return 
      mr1.ga==mr2.ga &&
      mr1.ca==mr2.ca &&
      mr1.ra==mr2.ra &&
      mr1.ba==mr2.ba &&
      mr1.ha==mr2.ha &&
      mr1.c_aggr_list == mr2.c_aggr_list &&
      mr1.sup<=mr2.sup &&
      //      mr1.conf<=mr2.conf &&
      mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
      mr1.headCardinalities.contains(mr2.headCardinalities) && 
      mr1.tab_source==mr2.tab_source &&
      PredicateUtils::predicatesAreEquivalent( mr1pmc, mr2pmc, mr1.tab_source ) &&
      PredicateUtils::predicatesAreEquivalent( mr1pgc, mr2pgc, mr1.tab_source ) &&
      PredicateUtils::predicatesAreEquivalent( mr1pcc, mr2pcc, mr1.tab_source );
  }
  

  bool OptimizedMinerule::firstMineruleDominatesSecondMinerule(
				 const ParsedMinerule& mr1,
				 const ParsedMinerule& mr2) {
#ifdef MRUSERWARNING
#warning Bisognerebbe controllare anche body==head, ma in questo \
momento body e head sono "fusi" in ra.
#endif

    
    Predicate mr1pmc(mr1.mc);
    Predicate mr2pmc(mr2.mc);
    Predicate mr1pgc(mr1.gc);
    Predicate mr2pgc(mr2.gc);
    Predicate mr1pcc(mr1.cc);
    Predicate mr2pcc(mr2.cc);

    PredicateUtils::PredicateRelationship mcrel = 
      PredicateUtils::getPredicateRelationship( mr1pmc, mr2pmc, mr1.tab_source );
    PredicateUtils::PredicateRelationship gcrel = 
      PredicateUtils::getPredicateRelationship( mr1pgc, mr2pgc, mr1.tab_source );
    PredicateUtils::PredicateRelationship ccrel = 
      PredicateUtils::getPredicateRelationship( mr1pcc, mr2pcc, mr1.tab_source );


    return 
      mr1.ga==mr2.ga &&
      mr1.ca==mr2.ca &&
      mr1.ra==mr2.ra &&
      mr1.ba==mr2.ba &&
      mr1.ha==mr2.ha &&
      mr1.c_aggr_list == mr2.c_aggr_list &&
      mr1.sup<=mr2.sup &&
      //      mr1.conf<=mr2.conf &&
      mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
      mr1.headCardinalities.contains(mr2.headCardinalities) && 
      mr1.tab_source==mr2.tab_source &&
      (mcrel==EncodedNF::FirstMoreGeneral || mcrel==EncodedNF::Equivalent)&&
      (gcrel==EncodedNF::FirstMoreGeneral || gcrel==EncodedNF::Equivalent)&&
      (ccrel==EncodedNF::FirstMoreGeneral || ccrel==EncodedNF::Equivalent);
  }

  OptimizedMinerule::MineruleRelationship
  OptimizedMinerule::getMineruleRelationship(const ParsedMinerule& mr1,
					     const ParsedMinerule& mr2) {
    if( MineruleOptions::getSharedOptions().getOptimizations().getAvoidDominanceDetection() )
      return None;
#ifdef MRUSERWARNING
#warning Bisognerebbe controllare anche body==head, ma in questo \
momento body e head sono "fusi" in ra.
#endif
    if(!(mr1.ga==mr2.ga &&
	 mr1.ca==mr2.ca &&
	 mr1.ra==mr2.ra &&
	 mr1.ba==mr2.ba &&
	 mr1.ha==mr2.ha &&
	 mr1.c_aggr_list == mr2.c_aggr_list &&
	 mr1.sup<=mr2.sup &&
	 //	 mr1.conf<=mr2.conf &&
	 mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
	 mr1.headCardinalities.contains(mr2.headCardinalities) && 
	 mr1.tab_source==mr2.tab_source))
      return None;

    Predicate mr1pmc(mr1.mc);
    Predicate mr2pmc(mr2.mc);
    Predicate mr1pgc(mr1.gc);
    Predicate mr2pgc(mr2.gc);
    Predicate mr1pcc(mr1.cc);
    Predicate mr2pcc(mr2.cc);

    PredicateUtils::PredicateRelationship relations[3];

    relations[0] = PredicateUtils::getPredicateRelationship( mr1pmc, mr2pmc, mr1.tab_source );
    relations[1] = PredicateUtils::getPredicateRelationship( mr1pgc, mr2pgc, mr1.tab_source );
    relations[2] = PredicateUtils::getPredicateRelationship( mr1pcc, mr2pcc, mr1.tab_source );

    MineruleRelationship current = Equivalence;
    
    for(size_t i=0; i<3; i++) {
      switch( relations[i] ) {
      case EncodedNF::FirstMoreGeneral:
	current=Dominance;
	break;
      case EncodedNF::Equivalent:
	// Nothing to do, just avoid to select the default
	// label
	break;
      default:
	return None;
      };
    }

    return current;
  }
  

  

} // end namespace minerule