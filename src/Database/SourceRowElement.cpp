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
#include "minerule/Database/SourceRowElement.hpp"
#include "minerule/Database/SourceRowAttribute.hpp"
#include "minerule/Database/SourceRowAttributeCollection.hpp"

namespace minerule {

	SourceRowElement* 
		SourceRowElement::createElement(
			mrdb::ResultSetMetaData* rsmd,
				mrdb::ResultSet* rs,
	const std::vector<int>& srd) {
		if(srd.empty())
			return NULL;
  
		if(srd.size()==1) {
			return SourceRowAttribute::createAttribute(rsmd,rs,srd[0]);
		}
		else 
			return new SourceRowAttributeCollection(rsmd,rs,srd);
	}

	SourceRowElement* 
	SourceRowElement::createElementFromType(const ElementType el) {
		if( SourceRowEmptyElement().getElementType()==el )
			return new SourceRowEmptyElement();

		if( GenericSourceRowAttribute().getElementType()==el )
			return new GenericSourceRowAttribute();

		if( MemDebugGenericSourceRowAttribute().getElementType()==el )
			return new MemDebugGenericSourceRowAttribute();

		if( NumericSourceRowAttribute().getElementType()==el )
			return new NumericSourceRowAttribute();

		if( SourceRowAttributeCollection().getElementType()==el)
			return new SourceRowAttributeCollection();

		throw MineruleException(MR_ERROR_INTERNAL, 
			"SourceRowElement::createElementFromType: "
				"Cannot recognize the specified element type!"); 
	}

	void SourceRowElement::serializeElementToString(const SourceRowElement& elem, std::string& strRepr) {
		std::ostringstream sstream;
		sstream << elem.getElementType();
		elem.serialize(sstream);
		strRepr = sstream.str();
	}

	SourceRowElement*
	SourceRowElement::deserializeElementFromResultSet(mrdb::ResultSet* rs, size_t start_index)  {
		mrdb::ResultSetMetaData* rsmd = rs->getMetaData();
		size_t numColumns = rsmd->getColumnCount();
		std::vector<int> descr;
		
		for(size_t i=start_index; i<=numColumns; ++i)
			descr.push_back(i);
		
		SourceRowElement* elem = SourceRowElement::createElement(rsmd, rs, descr);
		return elem;
	}


	SourceRowElement*
	SourceRowElement::deserializeElementFromString(const std::string& strRepr)  {
		std::istringstream sstream(strRepr);
		assert(sstream);
		ElementType elType;
		sstream>>elType;
		assert(sstream);

		SourceRowElement* elem = createElementFromType(elType);
		elem->deserialize(sstream);
		return elem;
	}

} // nameapace
