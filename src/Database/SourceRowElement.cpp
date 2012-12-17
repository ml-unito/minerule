#include "minerule/Database/SourceRowElement.h"
#include "minerule/Database/SourceRowAttribute.h"
#include "minerule/Database/SourceRowAttributeCollection.h"

namespace minerule {

	SourceRowElement* 
		SourceRowElement::createElement(
			odbc::ResultSetMetaData* rsmd,
				odbc::ResultSet* rs,
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
	SourceRowElement::createElementFromType(const ElementType el) throw(MineruleException) {
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

	void SourceRowElement::serializeElementToString(const SourceRowElement& elem, std::string& strRepr) throw(MineruleException) {
		std::ostringstream sstream;
		sstream << elem.getElementType();
		elem.serialize(sstream);
		strRepr = sstream.str();
	}

	SourceRowElement*
	SourceRowElement::deserializeElementFromResultSet(odbc::ResultSet* rs, size_t start_index) throw(MineruleException){
		odbc::ResultSetMetaData* rsmd = rs->getMetaData();
		size_t numColumns = rsmd->getColumnCount();
		std::vector<int> descr;
		
		for(size_t i=start_index; i<=numColumns; ++i)
			descr.push_back(i);
		
		SourceRowElement* elem = SourceRowElement::createElement(rsmd, rs, descr);
		return elem;
	}


	SourceRowElement*
	SourceRowElement::deserializeElementFromString(const std::string& strRepr) throw(MineruleException){
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
