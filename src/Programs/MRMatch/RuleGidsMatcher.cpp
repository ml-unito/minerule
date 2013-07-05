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
#include "RuleGidsMatcher.h"
#include "minerule/Database/Transaction.h"
#include "minerule/Result/RulesMatcher.h"
#include "minerule/Result/RuleFormatter.h"

using namespace minerule;

namespace mrmatch {
	
	// create a new empty rule and returns it (so that it can be intialized)
	Rule& RuleGidsMatcher::addRule() {
		_ruleGidsVector.push_back( RuleGids() );
		return _ruleGidsVector.back().first;
	}
	
	void RuleGidsMatcher::matchItemTransaction(const ItemType& gid, const ItemTransaction<RulesMatcher::ItemSetType>& bodies,const ItemTransaction<RulesMatcher::ItemSetType>& heads) {
			for(RuleGidsVector::iterator ruleIt = _ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {				
				if( RulesMatcher::match( ruleIt->first, bodies, heads ) ) {
					ruleIt->second.push_back( gid );
				}
			}		
	}
	
	void RuleGidsMatcher::matchRuleTransaction(const minerule::ItemType& gid, const RuleTransaction<RulesMatcher::RuleSetType>& transaction) {
			for(RuleGidsVector::iterator ruleIt=_ruleGidsVector.begin(); ruleIt!=_ruleGidsVector.end(); ++ruleIt) {
				if( RulesMatcher::match(ruleIt->first, transaction) ) {
					ruleIt->second.push_back( gid );
				}
			}		
	}
		
	
	
	std::string RuleGidsMatcher::formatGids( const Gids& gids ) const {
		std::stringstream str;
		for(std::vector<ItemType>::const_iterator it = gids.begin(); it!=gids.end(); ++it) {
			str << *it << " ";
		}
		
		return StringUtils::toBold(StringUtils::toGreen(str.str()));
	}
	
	void RuleGidsMatcher::printMatches( ) const {
		SimpleRuleFormatter sf;
		sf.setFieldWidths( SimpleRuleFormatter::FieldWidths(1,1,1,1) );
		for(RuleGidsVector::const_iterator it=_ruleGidsVector.begin(); it!=_ruleGidsVector.end(); ++it) {
			MRLog() 	<< StringUtils::toBold("Rule: ") << sf.formatRule(it->first) << " "
						<< StringUtils::toBold("\tGids: ") << formatGids(it->second) << std::endl;
		}
	}
	
	

}