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
#ifndef __ITEMTYPE_H__
#define __ITEMTYPE_H__

#include <iostream>
#include "minerule/Database/SourceRowElement.h"

namespace minerule {
  /**
	* The ItemType object is needed in order to make the algorithms to
	* work with ItemType instead of with pointers. The memory is handled
	* by this class in a very simplistic manner. It would be of much use to
	* reimplement the class so that it uses smart pointers instead of copying
	* again and again the actual objects.
   */


	class ItemType; // forward declaration
  
  /**
	* this type is the one used by procedures which deal with itemsets and rules
   */
	typedef std::vector<ItemType> ItemSet;
  

	class ItemType {
		friend std::ostream& operator<<(std::ostream&,const ItemType&);
		SourceRowElement* el;
	public:
		ItemType() : el(NULL) {
		}

		virtual ~ItemType() {
			if(el!=NULL)
				delete el;
		}

		ItemType( const ItemType& i)  {
			if( i.el==NULL)
				el=NULL;
			else
				el=i.el->copy();
		}

		// This constructor makes a copy of the given srel. The caller is free to delete it
		// as needed.
		ItemType( const SourceRowElement& srel ) {
			el = srel.copy();
		}
		
		// By calling this constructor one delegates to this class the management of srel.
		ItemType(SourceRowElement* srel) : el(srel) {}
		
		virtual void setPreparedStatementParameters(odbc::PreparedStatement* state, size_t start_index) const {
			el->setPreparedStatementParameters(state, start_index);
		}
		
		std::string getFullElementType() const { return el->getFullElementType(); }
    

    // Notice the srel given as argument to this function
    // will be managed by this object. In particular it
    // will be destroyed when the object is destroyed.
		void setSourceRowElement( SourceRowElement& srel ) {
			el = &srel;
		}

		ItemType& operator=(const ItemType& i) {
			if(el!=NULL) 
				delete el;

			if(i.el!=NULL)
				el = i.el->copy();
			else
				el = NULL;

			return *this;
		}

		ItemType& operator=(const SourceRowElement& i) {
			if(el!=NULL) 
				delete el;

			el = i.copy();

			return *this;
		}
 

		bool operator()(const ItemType& it1, const ItemType& it2) const {
			if(it1.el==NULL)
				return it2.el!=NULL;
      
			if(it2.el==NULL)
				return false;

			return *it1.el < *it2.el;
		}

		bool operator<(const ItemType& it) const {
			if(el==NULL)
				return it.el!=NULL;

			if(it.el==NULL)
				return false;
      
			return *el < *it.el;
		}

		bool operator<(const SourceRowElement& elem) const {
			if(el==NULL)
				return !elem.empty();

			if(elem.empty())
				return false;

			return *el < elem;
		}

		bool operator>(const ItemType& it) const {
			if(el==NULL)
				return false;

			if(it.el==NULL)
				return true;
      
			return *it.el < *el;
		}

		bool operator>(const SourceRowElement& elem) const {
			if(el==NULL)
				return false;

			if(elem.empty())
				return true;

			return elem < *el;
		}

		bool operator==(const ItemType& it) const {
			if(el==NULL)
				return it.el==NULL;

			if(it.el==NULL)
				return false;

			return *el == *it.el;
		}

		bool operator==(const SourceRowElement& elem) const {
			if(el==NULL)
				return elem.empty();

			if(elem.empty())
				return false;

			return *el == elem;
		}

		bool operator!=(const ItemType& it) const {
			if(el==NULL)
				return it.el!=NULL;

			if(it.el==NULL)
				return true;

			return *el != *it.el;
		}

		bool operator!=(const SourceRowElement& elem) const {
			if(el==NULL)
				return !elem.empty();

			if(elem.empty())
				return true;

			return *el != elem;
		}

		std::string getSQLData() const {
			assert(el!=NULL);
			return el->getSQLData();
		}

		std::string asString() const {
			assert(el!=NULL);
			return el->asString();
		}

		minerule::SourceRowElement& getElement() const throw(MineruleException) {
			if(el==NULL)
				throw MineruleException(MR_ERROR_INTERNAL, "Accessing a NULL element!");
			return *el;
		}

		static bool lessThan( const ItemType&, const ItemType&);

    
	};

	inline std::ostream& operator<<(std::ostream& os, const ItemType& it) {
		if(it.el==NULL) 
			os<<"NULL";
		else 
			os << *it.el;

		return os ; 
	}


	inline bool ItemType::lessThan( const ItemType& it1, const ItemType& it2) {
		return it1<it2;
	}

} // namespace
#endif
