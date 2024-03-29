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
#include "Matcher.hpp"
#include "RuleGidsMatcher.hpp"
#include "RuleGidsDBMatcher.hpp"
#include "GidRulesMatcher.hpp"
#include "minerule/Utils/MineruleErrors.hpp"

using namespace minerule;

namespace mrmatch {
	Matcher* Matcher::newMatcher(const Options& opts, const ParsedMinerule& minerule) {
		MatcherSpecs specs = opts.matcherSpecs();
		if((specs & MatchOutputMask) == OutOnDB) {
			return new RuleGidsDBMatcher(opts.getMatchOutputTableName(), minerule);
		}
		
		switch(specs & MatchKindMask) {
			case RuleGids: return new RuleGidsMatcher();
			case GidRules: return new GidRulesMatcher();
			default: throw MineruleException( MR_ERROR_INTERNAL, "Unknown or unsupported matcher kind");
		}
	}

	void Matcher::matchWithCrossProduct(SourceTable& st) {
		SourceTable::Iterator it = st.newIterator(SourceTable::FullIterator);
		
		while(!it.isAfterLast()) {
			ItemType gid = it->getGroup();
			
			RuleTransaction<RulesMatcher::RuleSetType> transaction;
			transaction.load(gid, it);
			
			matchRuleTransaction(gid, transaction);			
		}		
	}
		
	void Matcher::matchWithoutCrossProduct(SourceTable& st) {
		SourceTable::Iterator bodyIt = st.newIterator(SourceTable::BodyIterator);
		SourceTable::Iterator headIt = st.newIterator(SourceTable::HeadIterator);
				
		while(!bodyIt.isAfterLast()) {
			ItemType gid = bodyIt->getGroup();
			
			ItemTransaction<RulesMatcher::ItemSetType> bodies;
			ItemTransaction<RulesMatcher::ItemSetType> heads;
			
			bodies.loadBody(gid, bodyIt); 			// this advances the body iterator
						
			if( !TransactionBase<RulesMatcher::ItemSetType>::findGid(gid, headIt) ) {// positioning the head iterator  
				break;								// no more heads to load
			}

			heads.loadHead(gid, headIt);			// loading the heads

			matchItemTransaction(gid, bodies, heads);			
		} // while
		
	} // matchWithoutCrossProduct		

}