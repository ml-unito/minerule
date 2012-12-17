#include "minerule/Result/RulesMatcher.h"

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