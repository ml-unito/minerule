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


using namespace odbc;

namespace minerule {

	void OptimizedMinerule::attributesInPredicate( const list_OR_node* l, std::set<std::string>& result ) {
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
		std::set<std::string> mr1Attrs;
		std::set<std::string> mr2Attrs;

		attributesInPredicate( mr1.mc, mr1Attrs );
		attributesInPredicate( mr2.mc, mr2Attrs );

		return includes(mr2Attrs.begin(), mr2Attrs.end(), mr1Attrs.begin(), mr1Attrs.end());

	}

	// mr1: candidate query 
	// mr2: target query
	bool OptimizedMinerule::isACandidateQuery( const ParsedMinerule& mr1, const ParsedMinerule& mr2  ) { 
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
			OptimizerCatalogue::hasIDConstraints(mr1) &&
       	  // and the all the attributes on which mr1.mc predicates are included
     	  // in those on which mr2 predicates
			predicateAttributesAreIncluded(mr1,mr2);
		  // then mr1 is a candidate query
	}

	void OptimizedMinerule::checkForCombinedQueries(  ) {
		MRLogPush("Searching for equivalences due to query combinations...");
		std::vector<Predicate> candidates;
    
		std::vector<ParsedMinerule>::const_iterator it;

    // disabling the logging of the Parser since it would confuse
    // the user.
		MineruleOptions::getSharedOptions().getLogStreamObj().disable();

		std::ostream& log = MRLog() << "Candidates are: ";
    
		for(it=optInfo.minerulesToCombine.begin(); it!=optInfo.minerulesToCombine.end(); it++) {
			if(it!=optInfo.minerulesToCombine.begin()) log<<", ";

			log << it->tab_result;
			candidates.push_back(Predicate(it->mc));
		}
		log << std::endl;

    // re-enabling the log 
		MineruleOptions::getSharedOptions().getLogStreamObj().enable();

		Predicate target(minerule.mc);
    
	// FIXME: The third parameters ("Sales") seems to be obsoleted and not used any more
		GAQueryCombinator qCombinator(target, candidates);
		MineruleOptions::Optimizations::Combinator& combopt =
			MineruleOptions::getSharedOptions().getOptimizations().getCombinator();
	
		qCombinator.setMaxDisjuncts(combopt.getMaxDisjuncts());
		qCombinator.setMaxQueries(combopt.getMaxQueries());
		qCombinator.setMaxDistinctPredicates(combopt.getMaxDistinctPredicates());
		qCombinator.setTimeOutThreshold(combopt.getTimeOutThreshold());

		try {
			qCombinator.evolve();
		} catch( TimeOutException& e ) {
			MRLog() << "The algorithm timed out." << std::endl;
		}
    

	// FIXME: all of the following is about output. It should be moved into an appropriate method
		if( qCombinator.getResult().empty() ) {
			MRLog() << "No valid combination found!" << std::endl;
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
				<< " has been found. The found query follows:" << std::endl;
			MRLog() 	<< qryString << std::endl;

			optInfo.combinationFormula = qCombinator.getResult();
			optInfo.relationship = Combination;
		}
 
		MRLogPop();
	}


	void OptimizedMinerule::optimize()
	{
		if(minerule.miningTask!=MTMineRules) {
			MRLog() << "Skipping optimization, " << miningTaskToString(minerule.miningTask) << " task is still unsupported by the optimizer" << std::endl;
			MRDebug() << "Skipping optimization, " << miningTaskToString(minerule.miningTask) << " task is still unsupported by the optimizer" << std::endl;
			return;
		}

		std::list<ParsedMinerule> lPreviousMr;
		if(!MineruleOptions::getSharedOptions().getOptimizations().getTryOptimizations())
			return;

		bool avoidCombinationDetection = 
			MineruleOptions::getSharedOptions().getOptimizations().getAvoidCombinationDetection() 
				|| !hasIDConstraints();

		OptimizerCatalogue& catalogue = 
			MineruleOptions::getSharedOptions().getOptimizations().getCatalogue();

		QueryNormalizer normalizer( catalogue, minerule );
		normalizer.normalize();
		optInfo.minerule = minerule;
		optInfo.relationship = None;
		MRDebug("Minerule text after normalization:" + minerule.getText());

		std::vector<CatalogueInfo> catInfos;
		OptimizerCatalogue::getMRQueryInfos(catInfos);

    //lettura delle minerules precedenti
		try	{
			MRLogPusher _("Determining if a relationship exists with previous minerules...");
			
			MRLog() << "Reading past minerules" << std::endl;	
			for(std::vector<CatalogueInfo>::const_iterator it=catInfos.begin(); it!=catInfos.end(); ++it) {
				ParsedMinerule mr;
				mr.init(it->qryText);
				MRDebug("Optimize: trying minerule:" + mr.getText());

				if( !avoidCombinationDetection && isACandidateQuery(mr,minerule) ) {
					optInfo.minerulesToCombine.push_back(mr);
				}

				MineruleRelationship currentRel = getMineruleRelationship(mr,minerule);

				if( currentRel==Equivalence && MineruleOptions::getSharedOptions().getOptimizations().getAvoidEquivalenceDetection() )
						currentRel=Dominance;

				switch( currentRel ) {
					case Equivalence:
						MRLog() << "A past minerule found which includes the current one!" << std::endl;
						MRLog() << "Name of the found minerule:" << it->qryName << std::endl;
						MRLog() << "(The algorithm will store this information in the catalogue" << std::endl;
						MRLog() << " and exit)" << std::endl;
						optInfo.relationship = Equivalence;
						optInfo.minerule = mr;
						break;
					case Dominance:
						{
							MRLog() << "Minerule named ``" << mr.tab_result <<"'' dominates the current one." <<std::endl;
							if(optInfo.relationship==None) {
			// since relationship==None, there is not yet any information
			// about other Dominances. We simply store the found minerule
								optInfo.minerule = mr;
								MRLog() << "Minerule " << "``" << mr.tab_result <<"'' is now the best promising" 
									<< " dominant minerule." << std::endl;
							} else {
								// we must keep the current minerule only if its result set
								// is smaller than the one of the past Dominant query found
								CatalogueInfo oldQryInfo;
								OptimizerCatalogue::getMRQueryInfo( optInfo.minerule.tab_result, oldQryInfo );
								if( it->resSize<oldQryInfo.resSize) {
									optInfo.minerule = mr;
									MRLog() << "Minerule " << "``" << mr.tab_result <<"'' is now the best promising" 
											<< " dominant minerule." << std::endl;
								} else {
									MRLog() << "Keeping the previously found dominant minerule" << std::endl;
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
				MRLog() << "No interesting past minerules found." << std::endl;

			if(optInfo.relationship==Dominance) 
				MRLog() << "The best promising dominant minerule found is ``" 
					<< optInfo.minerule.tab_result << "''" << std::endl;	
		} catch (odbc::SQLException& e) {
			MRErr() << e.what() << std::endl;
			throw;
		}

		if(optInfo.relationship==None && !avoidCombinationDetection) {
			checkForCombinedQueries();
			if(optInfo.relationship==None)
				optInfo.minerulesToCombine.clear();
		}
	}




	bool OptimizedMinerule::firstMineruleIncludesSecondMinerule(const ParsedMinerule& mr1,const ParsedMinerule& mr2) {
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
			mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
			mr1.headCardinalities.contains(mr2.headCardinalities) && 
			mr1.tab_source==mr2.tab_source &&
			PredicateUtils::predicatesAreEquivalent( mr1pmc, mr2pmc, mr1.tab_source ) &&
			PredicateUtils::predicatesAreEquivalent( mr1pgc, mr2pgc, mr1.tab_source ) &&
			PredicateUtils::predicatesAreEquivalent( mr1pcc, mr2pcc, mr1.tab_source );
	}
  

	bool OptimizedMinerule::firstMineruleDominatesSecondMinerule(const ParsedMinerule& mr1,const ParsedMinerule& mr2) {

    
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
			mr1.bodyCardinalities.contains(mr2.bodyCardinalities) &&
			mr1.headCardinalities.contains(mr2.headCardinalities) && 
			mr1.tab_source==mr2.tab_source &&
			(mcrel==EncodedNF::FirstMoreGeneral || mcrel==EncodedNF::Equivalent)&&
			(gcrel==EncodedNF::FirstMoreGeneral || gcrel==EncodedNF::Equivalent)&&
			(ccrel==EncodedNF::FirstMoreGeneral || ccrel==EncodedNF::Equivalent);
	}

	OptimizedMinerule::MineruleRelationship
	OptimizedMinerule::getMineruleRelationship(const ParsedMinerule& mr1, const ParsedMinerule& mr2) {
		if( MineruleOptions::getSharedOptions().getOptimizations().getAvoidDominanceDetection() )
			return None;

		if(!(mr1.ga==mr2.ga &&
			mr1.ca==mr2.ca &&
			mr1.ra==mr2.ra &&
			mr1.ba==mr2.ba &&
			mr1.ha==mr2.ha &&
			mr1.c_aggr_list == mr2.c_aggr_list &&
			mr1.sup<=mr2.sup &&
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
  

	bool OptimizedMinerule::hasIDConstraints() const throw(MineruleException) {
		return OptimizerCatalogue::hasIDConstraints(minerule);
	}
  

} // end namespace minerule
