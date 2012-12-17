#ifndef RULESMATCHER_H_2OI7RAD0
#define RULESMATCHER_H_2OI7RAD0

#include "minerule/Database/Transaction.h"
#include "minerule/Result/Rule.h"

namespace minerule {
	class RulesMatcher {
	public:
		typedef std::set<ItemType> ItemSetType;
		typedef std::set< std::pair<ItemType, ItemType> > RuleSetType;
		
		static bool match( const Rule& r, const RuleTransaction<RuleSetType>& t );		
		static bool match( const Rule& r, const ItemTransaction<ItemSetType>& bodySet, const ItemTransaction<ItemSetType>& headSet );		
	};

}

#endif /* end of include guard: RULESMATCHER_H_2OI7RAD0 */
