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
#ifndef __ITEMATTRIBUTE_H__
#define __ITEMATTRIBUTE_H__

#include <string.h>
#include <odbc++/resultset.h>
#include <odbc++/preparedstatement.h>
#include <odbc++/resultsetmetadata.h>
#include <string>
#include <iostream>

#include "minerule/Database/SourceRowElement.h"

namespace minerule { 
  /**
   * This is the base class for all attributes types. It should be subclassed
   * in order to provide the actual implementation needed by each type.
   * For instance a NumericalAttribute should exist in order to provide 
   * methods specifically tuned to handle numerical data.
   */
  class SourceRowAttribute : public SourceRowElement {
  public:
    /**
     * Factory method - returns an attribute capable of handling types of the given type
     */

    static
      SourceRowAttribute* createAttribute(odbc::ResultSetMetaData* rsmd, odbc::ResultSet* rs, int elem);
    

    virtual ~SourceRowAttribute() {
    }

    // ---- Methods to be implemented in sub-classes
    /**
     * returns a fresh allocated copy of the current object
     */
    virtual SourceRowElement* copy() const =0;

    /**  
     * return the type of the attribute 
     */
    virtual odbc::Types::SQLType 
      getType() const = 0;

    /**
     * return the id of the column from which the attribute where extracted
     * deprecated... it is of no practical interest and it forces subclasses
     * to store an additional integer
     */
    // virtual int getColumnId() const = 0;

    /** 
     * set the value of the attribute accordingly to the value of the specified
     * column of the current row of the given result set
     */
    virtual void setValue( odbc::ResultSet*, int)=0;
    virtual void setValue( const std::string& )=0;

    /**
     * Return -1 if the present attribute is less than the argument, 0 if they are equal
     * +1 otherwise
     */
    virtual int compareTo(const SourceRowAttribute&) const =0;

    /** 
     * return thestd::string representation for the attribute
     */ 
    virtual std::string asString(const std::string& sep=",") const = 0;

    // INHERITED FROM SOURCE ROW ELEMENT

    // Returns true if s1<s2
    virtual bool operator()(const SourceRowElement& s1, 
			    const SourceRowElement& s2) const {
      const SourceRowAttribute& attr1 = 
	dynamic_cast<const SourceRowAttribute&>(s1);
      const SourceRowAttribute& attr2 =
	dynamic_cast<const SourceRowAttribute&>(s2);

      return attr1.compareTo(attr2)<0;
    }

    virtual bool operator==(const SourceRowElement& s1) const {
       const SourceRowAttribute& attr1 = dynamic_cast<const SourceRowAttribute&>(s1);

       return this->compareTo(attr1)==0;
    }


    virtual bool empty() const {
      return false;
    }

    /**
     * return thestd::string representation for the attribute in a format
     * suitable to be used in SQL queries (tipically it is implemented
     * as return "'"+this->asString()+"'"
     */
    virtual std::string
      getSQLData() const = 0;

    virtual std::ostream& operator<<(std::ostream& os) const {
      os << asString();
      return os;
    }
  };


  /**
   * This is a generic attribute class, i.e. it can contain any
   * attribute type. It should be used when more specific class
   * are not available.
   */
  
  class GenericSourceRowAttribute : public SourceRowAttribute {
    friend 
      std::ostream& operator<<(std::ostream& os, const SourceRowAttribute& ia);
  private:
    std::string value;

    odbc::Types::SQLType type;
    // Deprecated 
    // int colId;
  public:
    GenericSourceRowAttribute(odbc::ResultSet* _rs, int _elem, odbc::Types::SQLType _type) : 
       value(_rs->getString(_elem)),type(_type) {
      
    }


    GenericSourceRowAttribute() : value(""), 
      type((odbc::Types::SQLType)0)
      /*, colId(-1) */{
    };

    // copy constructor
    GenericSourceRowAttribute(const GenericSourceRowAttribute& rhs) 
      : value(rhs.value), type(rhs.type) /*, colId(rhs.colId)*/ {
    }

    virtual ~GenericSourceRowAttribute() {
      //      std::cout << "Generic... destructor" << std::endl;
    }

    // Implementing AttributeType methods...

    virtual SourceRowElement* copy() const {
      SourceRowAttribute* newRow= new GenericSourceRowAttribute(*this);
      return newRow;
    }
	
	virtual void setPreparedStatementParameters(odbc::PreparedStatement* state, size_t start_index) const {
		state->setString(start_index, value);
	}
	

    virtual odbc::Types::SQLType 
      getType() const;

    // Deprecated
    // virtual int getColumnId() const;

    virtual void setValue( odbc::ResultSet*, int);
    virtual void setValue( const std::string& );
    virtual int compareTo(const SourceRowAttribute&) const;

    virtual std::string asString(const std::string& sep=",") const;

    virtual std::string
       getSQLData() const;

    virtual SourceRowElement&
      operator=(const SourceRowElement& _rhs) {
      const GenericSourceRowAttribute& rhs = 
	dynamic_cast<const GenericSourceRowAttribute&>(_rhs);
      value = rhs.value;
      type = rhs.type;
      return *this;
    }

    /**
     * Serialization methods
     */
    virtual ElementType getElementType() const {
      return 'g';
    }
    virtual void serialize(std::ostream& os) const throw(MineruleException);
    virtual void deserialize(std::istream& is) throw(MineruleException);
  };

  /** 
   * This class keep track of how many instances has been created. It is useful
   * in order to check if a program deallocate sourcerowattribute instances.
   */

  class MemDebugGenericSourceRowAttribute : public GenericSourceRowAttribute {
    static size_t instanceCount;
  private:
    void incCounter() {
      instanceCount++;
      //      std::cout << "Created new SourceRowAttribute. Count after creation:" 
      //	<< instanceCount << std::endl;
    }

    void decCounter() {
      instanceCount--;
      // std::cout << "Deleted a SourceRowAttribute. Count after deletion:" 
      //	<< instanceCount << std::endl;
    }
  public:
    MemDebugGenericSourceRowAttribute() : GenericSourceRowAttribute() {
      incCounter();
    }

    MemDebugGenericSourceRowAttribute(odbc::ResultSet* _rs,
				      int _elem,
				      odbc::Types::SQLType _type) : GenericSourceRowAttribute(_rs,_elem,_type) {
      incCounter();
    }

    MemDebugGenericSourceRowAttribute(const MemDebugGenericSourceRowAttribute& rhs) 
      : GenericSourceRowAttribute(rhs) {
      incCounter();
    }

    virtual ~MemDebugGenericSourceRowAttribute() {
      decCounter();
    }
	
    
    virtual SourceRowElement* copy() const {
      SourceRowAttribute* newRow= new MemDebugGenericSourceRowAttribute(*this);
      return newRow;
    }

    static size_t getInstanceCounter()  {
      return instanceCount;
    }
  };


  /* NUMERIC SOURCE ROW ATTRIBUTE */
  class NumericSourceRowAttribute : public SourceRowAttribute {
    friend 
      std::ostream& operator<<(std::ostream& os, const SourceRowAttribute& ia);
  private:
    long int value;


  public:
    const static SourceRowAttribute::ElementType elementType;

    NumericSourceRowAttribute( odbc::ResultSet* _rs,
			       int _elem ) : value( _rs->getInt(_elem)) {

    }

    NumericSourceRowAttribute() : value(0) {
    };

    // copy constructor
    NumericSourceRowAttribute(const NumericSourceRowAttribute& rhs) 
      : value(rhs.value) {
    }

    virtual ~NumericSourceRowAttribute() {
      //      std::cout << "Numeric... destructor" << std::endl;
    }

    // Implementing AttributeType methods...

    virtual SourceRowElement* copy() const {
      SourceRowAttribute* newRow= new NumericSourceRowAttribute(*this);
      return newRow;
    }
	
	virtual void setPreparedStatementParameters(odbc::PreparedStatement* state, size_t start_index) const {
		state->setLong(start_index, value);
	}
	

    virtual odbc::Types::SQLType 
      getType() const {
      return odbc::Types::INTEGER;
    }

    virtual void setValue( odbc::ResultSet*, int);
    virtual void setValue( const std::string& );
    virtual int compareTo(const SourceRowAttribute&) const;

    virtual std::string asString(const std::string& sep=",") const;

    virtual std::string
       getSQLData() const;

    virtual SourceRowElement&
      operator=(const SourceRowElement& _rhs) {
      const NumericSourceRowAttribute& rhs =
	dynamic_cast<const NumericSourceRowAttribute&>(_rhs);
      value = rhs.value;
      return *this;
    }

    
    /**
     * Serialization methods
     */
    virtual ElementType getElementType() const {
      return 'n';
    }
    virtual void serialize(std::ostream& os) const throw(MineruleException);
    virtual void deserialize(std::istream& is) throw(MineruleException);
  };


  inline std::ostream& operator<<(std::ostream& os, const SourceRowAttribute& attr) {
    attr.operator<<(os);
    return os;
  } 

}  // minerule;


#endif
