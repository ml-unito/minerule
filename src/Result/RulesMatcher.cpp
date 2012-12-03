#include "Result/RulesMatcher.h"

namespace minerule {
	bool RulesMatcher::match( const Rule& r, const RuleTransaction<SetType>& t ) {
		assert(false /*not implmented yet*/);
	}
	
	bool RulesMatcher::match( const Rule& r, const ItemTransaction<SetType>& bodySet, const ItemTransaction<SetType>& headSet ) {
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