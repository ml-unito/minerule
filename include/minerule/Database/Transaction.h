//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Marco Botta (botta@di.unito.it) and Roberto Esposito (esposito@di.unito.it)
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
#ifndef ITEMSETLOADER_H_J9K5VPNF
#define ITEMSETLOADER_H_J9K5VPNF

#include "minerule/Database/SourceTable.h"

namespace minerule {
	// --------------------------------------------------------------------------------
	// Classes declared here load source tables into memory in group sized chunks. 
	// --------------------------------------------------------------------------------
	
	// Base class. Provides the findGid method.
	template <class SetType>
	class TransactionBase : public SetType {
	public:
		TransactionBase() : SetType() {}
					
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
	class ItemTransaction : public TransactionBase<ItemSetType> {
	public:
		ItemTransaction() : TransactionBase<ItemSetType>() {}
			
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
	class RuleTransaction : public TransactionBase<RuleSetType> {
	public:
		RuleTransaction() : TransactionBase<RuleSetType>() {}
			
	    void load(ItemType& gid, SourceTable::Iterator& it) {
				while (!it.isAfterLast() && gid == it->getGroup()) {
					RuleSetType::insert(RuleSetType::end(),std::pair<ItemType,ItemType>(it->getBody(),it->getHead()));
					++it;				
				}
	    }
	};
	
	
}

#endif /* end of include guard: ITEMSETLOADER_H_J9K5VPNF */
