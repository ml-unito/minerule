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
#ifndef RULEGIDMATCHER_H_X9STRBC4
#define RULEGIDMATCHER_H_X9STRBC4

#include "Matcher.hpp"

namespace mrmatch {

	class RuleGidsMatcher : public Matcher {
	protected:
		typedef std::vector<minerule::ItemType> Gids;
		typedef std::pair< minerule::Rule, Gids > RuleGids;
		typedef std::vector< RuleGids > RuleGidsVector;
		RuleGidsVector _ruleGidsVector;
		
		virtual void matchItemTransaction(const minerule::ItemType& gid, const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& bodies,const minerule::ItemTransaction<minerule::RulesMatcher::ItemSetType>& heads);
		virtual void matchRuleTransaction(const minerule::ItemType& gid, const minerule::RuleTransaction<minerule::RulesMatcher::RuleSetType>& transaction);

		std::string formatGids( const Gids& gids ) const;	
		
	public:
		RuleGidsMatcher() {}
		virtual ~RuleGidsMatcher() {}

	// create a new empty rule and returns it (so that it can be intialized)
		virtual minerule::Rule& addRule();
		
	// outputs the results
		virtual void printMatches() const;
	
	};

}

#endif /* end of include guard: RULEGIDMATCHER_H_X9STRBC4 */
