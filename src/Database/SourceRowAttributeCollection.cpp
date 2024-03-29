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
#include <typeinfo>
#include "minerule/Database/SourceRowAttributeCollection.hpp"
#include "minerule/Utils/MineruleOptions.hpp"

/* ******************************* */
/* SourceRowAttributeCollection methods */
/* ******************************* */

namespace minerule {

  // public members

  SourceRowAttributeCollection::SourceRowAttributeCollection(
				     mrdb::ResultSetMetaData* rsmd,
				     mrdb::ResultSet* rs,
				     std::vector<int> elems) {
    assert(!elems.empty());
    
    std::vector<int>::const_iterator it = elems.begin(); 
    for(; it!=elems.end(); it++) {
      SourceRowAttribute* attr = SourceRowAttribute::createAttribute(rsmd,rs,*it);
      attributes.push_back(attr);
    }
  }
  

  // copy constructor
  SourceRowAttributeCollection::SourceRowAttributeCollection(
			     const SourceRowAttributeCollection& rhs) {
    CollectionType::const_iterator it1 = rhs.attributes.begin();

    while( it1!=rhs.attributes.end() ) {
      attributes.push_back((SourceRowAttribute*)(*it1)->copy());
      it1++;
    }
  }

  SourceRowElement&
  SourceRowAttributeCollection::operator=(
			      const SourceRowElement& _rhs){
    try {
      const SourceRowAttributeCollection& rhs =
	dynamic_cast<const SourceRowAttributeCollection&>(_rhs);

      freeCollection();
      CollectionType::const_iterator it1 = rhs.attributes.begin();
      
      while( it1!=rhs.attributes.end() ) {
	attributes.push_back((SourceRowAttribute*)(*it1)->copy());
	it1++;
      }
    } catch (std::bad_cast& bc) {
      MRErr() << "Fatal error (SourceRowAttributeCollection::operator=)" << std::endl;
      throw;
    }

      return *this;
  }

  void
  SourceRowAttributeCollection::freeCollection() {
    //  std::cout << "freeCollection..." << std::endl;
    CollectionType::const_iterator it1 = attributes.begin();
    while( it1!=attributes.end() ) {
      delete *it1;
      it1++;
    } 

    if(attributes.size()>0)
      attributes.clear();
  }

  /** Strings related... */

  std::string
  SourceRowAttributeCollection::asString(const std::string& sep) const {
    CollectionType::const_iterator it;

    std::string attrString;

    attrString = ""; 

    //  std::cerr << "in sourcerowattrcollection::c_str collection size:"<< attributes->size() << std::endl;

    it=attributes.begin();
    if(it!=attributes.end()) {
      attrString+=(*it)->asString(sep);
      it++;
    }

    for(; it!=attributes.end();it++) {
      attrString+=sep;
      attrString+=(*it)->asString(sep);
    }

    return attrString;
  }
  


  void SourceRowAttributeCollection::setPreparedStatementParameters(mrdb::PreparedStatement* state, size_t start_index) const {
	  size_t count = start_index;
	  for( CollectionType::const_iterator it = attributes.begin(); it!=attributes.end(); ++it ) {
		  (*it)->setPreparedStatementParameters(state, count++);
	  }
  }

	

  std::string 
  SourceRowAttributeCollection::getSQLData() const {
    CollectionType::const_iterator it = attributes.begin();
   std::string result = "";

    if(it!=attributes.end()) {
      result+= (*it)->getSQLData();

      it++;
    }

    for(; it!=attributes.end(); it++ ) {
      result+="," + (*it)->getSQLData();
    }

    return result;
  }
  
  std::string SourceRowAttributeCollection::getFullElementType() const {
	  char chstr[2] = { getElementType(), '\0' };
	  std::string result( chstr );
	  
	  CollectionType::const_iterator it = attributes.begin();
	  for(; it!=attributes.end(); ++it ) {
		  // nested attributes collections cannot be created, it suffices
		  // to iterate over the attributes and collect types with getElementType
		  // (instead of getFullElementType).
	  	  result+=(*it)->getElementType(); 
	  }
	  
	  return result;
  }
  
  // operators

  bool 
  SourceRowAttributeCollection::operator==(const SourceRowElement& e2) const {
    const SourceRowAttributeCollection& s1 = *this;
    const SourceRowAttributeCollection& s2 = 
      dynamic_cast<const SourceRowAttributeCollection&>(e2);

    CollectionType::const_iterator it1 = s1.attributes.begin();
    CollectionType::const_iterator it2 = s2.attributes.begin();


    for(; it1!=s1.attributes.end() && 
	  it2!=s2.attributes.end() &&
	  (**it1)==(**it2); it1++, it2++ );

    return (it1==s1.attributes.end() && it2==s2.attributes.end());
  }
  // Returns true if s1<s2

  bool 
  SourceRowAttributeCollection::operator()(const SourceRowElement& e1,const SourceRowElement& e2) const  {
    const SourceRowAttributeCollection& s1 = 
      dynamic_cast<const SourceRowAttributeCollection&>(e1);
    const SourceRowAttributeCollection& s2 = 
      dynamic_cast<const SourceRowAttributeCollection&>(e2);

    CollectionType::const_iterator it1 = s1.attributes.begin();
    CollectionType::const_iterator it2 = s2.attributes.begin();


    for(; it1!=s1.attributes.end() && 
	  it2!=s2.attributes.end() &&
	  (**it1)==(**it2); it1++, it2++ );

    // here: it1 and it2 counted the same number of elements from the start of the respective
    //   containers, i.e. pos(it1)==pos(it2)==J for a given J. 
    // moreover: forall i<J s1[i]==s2[i] AND
    // (
    //  it1==it2 == end() (==> s1 == s2) OR
    //  it1==end() && it2!=end() (==> s1<s2)
    //  it1!=end() && it2!=end() && it1!=it2 (==> s1<s2 <=> (*it1)->compareTo(**it2)<0) )
    // )

    if(it1==s1.attributes.end() && it2==s2.attributes.end()) // s1==s2
      return false;

    if(it1==s1.attributes.end())  // s1<s2
      return true;

    if(it2==s2.attributes.end()) // s1>s2
      return false;


    return (**it1)<(**it2);
  }


  std::ostream& SourceRowAttributeCollection::operator<<(std::ostream& os) const {
    if(attributes.empty()) {
      os << "(attr_collection_empty)";
      return os;
    }
    
    SourceRowAttributeCollection::CollectionType::const_iterator it;
    it = attributes.begin();
    os << "attr_collection:(";
    
    os << **it;
    it++;
    
    while(it!=attributes.end()) {
      os << "," << **it;
      it++;
    }
    
    os <<")";

    return os;
  }


  void 
  SourceRowAttributeCollection::serialize(std::ostream& os) const  {
    SourceRowAttributeCollection::CollectionType::const_iterator it;
    for(it=attributes.begin(); it!=attributes.end(); it++) {
      os << " " << (*it)->getElementType() << " ";  // more elements to come, specifying type
      (*it)->serialize(os);
    }
    
    os << " S "; // stop!
  }
  
  void 
  SourceRowAttributeCollection::deserialize(std::istream& is) {
   std::string buf;
    is >> buf;
    while(buf!="S" && is) {
      if( buf.size()!=1 ) 
	throw MineruleException(MR_ERROR_INTERNAL, 
				"Expecting a single letter element class id, but found:"
				+buf);

      SourceRowAttribute* element = 
	dynamic_cast<SourceRowAttribute*>(SourceRowElement::createElementFromType(buf[0]));

      if(element==NULL)
	throw MineruleException(MR_ERROR_INTERNAL,
				"Cannot create a source row element identified by:" +
				std::string(buf));
      element->deserialize(is);

      attributes.push_back(element);
      is >> buf;
    }

    if(buf!="S")
      throw MineruleException(MR_ERROR_INTERNAL,
			      "Expecting a source row attribute, but the stream ended!");
  }


} // namespace minerule
