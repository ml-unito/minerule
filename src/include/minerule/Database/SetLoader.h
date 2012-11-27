#ifndef ITEMSETLOADER_H_J9K5VPNF
#define ITEMSETLOADER_H_J9K5VPNF

#include "Database/SourceTable.h"

namespace minerule {
	
	// --------------------------------------------------------------------------------
	// Classes declared here load source tables into memory in group sized chunks. 
	// --------------------------------------------------------------------------------

	
	// Base class. Provides the findGid method.
	template <class SetType>
	class SetLoader : public SetType {
	public:
		SetLoader() : SetType() {}
					
		static bool findGid(ItemType& gid, SourceTable::Iterator& it) {      
			while( !it.isAfterLast() && gid > it->getGroup() ) {
				++it;
			}
    
			return !it.isAfterLast() && gid == it->getGroup();
		}
	};
	
	
	// Loader for result set organized as item lists (i.e., the underlying
	// query does *not* need a dataset in the form of a joint table that 
	// differentiates between body and head attributes.
	template <class ItemSetType>
	class ItemSetLoader : public SetLoader<ItemSetType> {
	public:
		ItemSetLoader() : SetLoader<ItemSetType>() {}
			
		void loadBody(ItemType& gid, SourceTable::Iterator& it) {
			while (!it.isAfterLast() && gid == it->getGroup()) {

				ItemSetType::insert(it->getBody());
				++it;					
			}
		}

		void loadHead(ItemType& gid, SourceTable::Iterator& it) {
			while (!it.isAfterLast() && gid == it->getGroup()) {
					
				ItemSetType::insert(it->getHead());
				++it;
			}
		}
	};
	
	// Loader for result set organized as item lists (i.e., the underlying
	// query needs a dataset in the form of a joint table that differentiates between
	// body and head attributes.
	template <class RuleSetType>	
	class RuleSetLoader : public SetLoader<RuleSetType> {
	public:
		RuleSetLoader() : SetLoader<RuleSetType>() {}
			
	    void load(ItemType& gid, SourceTable::Iterator& it) {

			while (!it.isAfterLast() && gid == it->getGroup()) {
				RuleSetType::insert(RuleSetType::end(),std::pair<ItemType,ItemType>(it->getBody(),it->getHead()));
				++it;				
			}
	    }
	};
	
	
}

#endif /* end of include guard: ITEMSETLOADER_H_J9K5VPNF */
