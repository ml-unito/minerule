#ifndef ITEMSETLOADER_H_J9K5VPNF
#define ITEMSETLOADER_H_J9K5VPNF

namespace minerule {
	
	template <class ItemSetType>
	class ItemSetLoader : public ItemSetType {
	public:
		ItemSetLoader() : ItemSetType() {}
			
		void loadBody(ItemType& gid, SourceTable::Iterator it) {
			while (!it.isAfterLast() && gid == it->getGroup()) {

				ItemSetType::insert(it->getBody());
				++it;					
			}
		}

		void loadHead(ItemType& gid, SourceTable::Iterator it) {
			while (!it.isAfterLast() && gid == it->getGroup()) {
					
				ItemSetType::insert(it->getHead());
				++it;
			}
		}

		static bool findGid(ItemType& gid, SourceTable::Iterator it) {      
			while (!it.isAfterLast() && gid > it->getGroup() ) {
				++it;
			}
    
			return !it.isAfterLast() && gid == it->getGroup();
		}
	};

}

#endif /* end of include guard: ITEMSETLOADER_H_J9K5VPNF */
