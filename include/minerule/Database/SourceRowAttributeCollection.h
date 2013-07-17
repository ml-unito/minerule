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
#ifndef __SOURCEROWATTRIBUTECOLLECTION_H__
#define __SOURCEROWATTRIBUTECOLLECTION_H__

#include <vector>
#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/resultsetmetadata.h>

#include "minerule/Database/SourceRowAttribute.h"
#include "minerule/Database/SourceRowColumnIds.h"


namespace minerule {
/* This class is an helper of class SourceRow. It helps in 
 * maintaining parts of items.
 */

  class SourceRowAttributeCollection : public SourceRowElement {
  private:
    typedef std::vector<SourceRowAttribute*> CollectionType;
    // storage
    CollectionType attributes;

    void freeCollection(); // clear the collection

    static bool lessThan( const SourceRowAttributeCollection& s1, 
			  const SourceRowAttributeCollection& s2);
  public:
    // builds an empty attribute collection
    SourceRowAttributeCollection()  {
    }

    SourceRowAttributeCollection(
				 odbc::ResultSetMetaData* rsmd,
				 odbc::ResultSet* rs,
				 std::vector<int> elems);
    
    // copy constructor
    SourceRowAttributeCollection(const SourceRowAttributeCollection& rhs);


    // destructor
    virtual ~SourceRowAttributeCollection() {
	freeCollection();
    }

    virtual SourceRowElement&
      operator=(const SourceRowElement& rhs);
    
    virtual bool operator()(const SourceRowElement& s1, 
			    const SourceRowElement& s2) const;
    virtual bool operator==(const SourceRowElement&) const;
    
    virtual bool empty() const {
      return attributes.empty();
    }
	
	virtual void setPreparedStatementParameters(odbc::PreparedStatement* state, size_t start_index) const;
	
	
    
    virtual std::string getSQLData() const;

    virtual std::string asString(const std::string& sep=",") const;
    
    virtual SourceRowElement* copy() const {
      return new SourceRowAttributeCollection(*this);
    }
    
    virtual std::ostream& operator<<(std::ostream& os) const;


    virtual ElementType getElementType() const {
      return 'A';
    }
	
	virtual std::string getFullElementType() const;
	
    virtual void 
    serialize(std::ostream& os) const throw(MineruleException);
  
    virtual void 
    deserialize(std::istream& is) throw(MineruleException);
    
  };
}

#endif
