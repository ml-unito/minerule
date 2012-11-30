#ifndef RULESMATCHER_H_2OI7RAD0
#define RULESMATCHER_H_2OI7RAD0

#include "Database/Transaction.h"
#include "Result/Rule.h"

namespace minerule {
	class RulesMatcher {
	public:
		typedef std::set<ItemType> SetType;
		
		static bool match( const Rule& r, const RuleTransaction<SetType>& t );		
		static bool match( const Rule& r, const ItemTransaction<SetType>& bodySet, const ItemTransaction<SetType>& headSet );		
	};

}

#endif /* end of include guard: RULESMATCHER_H_2OI7RAD0 */
