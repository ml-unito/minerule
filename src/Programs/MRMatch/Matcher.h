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
#ifndef MATCHERS_H_Q12443YU
#define MATCHERS_H_Q12443YU

#include "minerule/Result/Rule.h"
#include "minerule/Database/SourceTable.h"
#include "minerule/Database/Transaction.h"
#include "minerule/Result/RulesMatcher.h"

#include "Options.h"
#include "mrmatch.h"

namespace mrmatch {

// Base class for all Matchers
	class Matcher {
	public:
	// creates a new matcher and returns it
		static Matcher* newMatcher(const Options& opts, const minerule::ParsedMinerule& p);

	// create a new empty rule and returns it (so that it can be intialized)
		virtual minerule::Rule& addRule() = 0;
	
	// Match the given source table with the current set of rules
		virtual void match(minerule::SourceTable& st) {
			st.usesCrossProduct() ? matchWithCrossProduct(st) : matchWithoutCrossProduct(st);
		}
		
		virtual void matchWithCrossProduct(minerule::SourceTable& st);
		virtual void matchWithoutCrossProduct(minerule::SourceTable& st);		
		
	// outputs the results
		virtual void printMatches() const = 0;
	
	protected:
		virtual void matchItemTransaction(const minerule::ItemType& gid, const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& bodies,const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& heads) = 0;
		virtual void matchRuleTransaction(const minerule::ItemType& gid, const minerule::RuleTransaction<minerule::RulesMatcher::RuleSetType>& transaction) = 0;
	};

}

#endif /* end of include guard: MATCHERS_H_Q12443YU */
