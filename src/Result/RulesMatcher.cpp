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
#include "minerule/Result/RulesMatcher.hpp"

namespace minerule {	
	bool RulesMatcher::match( const Rule& r, const RuleTransaction<RuleSetType>& t ) {
		std::set< ItemType > bodySet;
		std::set< ItemType > headSet;
		
		for( RuleSetType::const_iterator it = t.begin(); it!=t.end(); ++it ) {
			bodySet.insert(it->first);
			headSet.insert(it->second);
		}
		
		const ItemSet& body = r.getBody();
		for( ItemSet::const_iterator bodyIt=body.begin(); bodyIt!=body.end();++bodyIt) {
			if(bodySet.find(*bodyIt) == bodySet.end())
				return false;
		}
		
		const ItemSet& head = r.getHead();
		for( ItemSet::const_iterator headIt=head.begin(); headIt!=head.end();++headIt) {
			if(headSet.find(*headIt) == headSet.end())
				return false;
		}
		
		return true;
	}
	
	bool RulesMatcher::match( const Rule& r, const ItemTransaction<ItemSetType>& bodySet, const ItemTransaction<ItemSetType>& headSet ) {
		const ItemSet& body = r.getBody();
		for( ItemSet::const_iterator it=body.begin(); it!=body.end();++it) {
			if(bodySet.find(*it) == bodySet.end())
				return false;
		}
		
		const ItemSet& head = r.getHead();
		for( ItemSet::const_iterator it=head.begin(); it!=head.end();++it) {
			if(headSet.find(*it) == headSet.end())
				return false;
		}
		
		return true;
	}
}