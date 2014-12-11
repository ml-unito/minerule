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
#include <iostream>

#include "minerule/mrdb/ResultSet.hpp"
#include "minerule/mrdb/PreparedStatement.hpp"
#include "minerule/mrdb/ResultSetMetaData.hpp"
#include "minerule/mrdb/Types.hpp"

#include "minerule/Database/SourceRowElement.hpp"

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
   * Factory method.
   * @return an attribute capable of handling types of the
   * given type
   */

  static SourceRowAttribute *createAttribute(mrdb::ResultSetMetaData *rsmd,
                                             mrdb::ResultSet *rs, int elem);

  virtual ~SourceRowAttribute() {}

  // ---- Methods to be implemented in sub-classes
  /**
   * @return a fresh allocated copy of the current object
   */
  virtual SourceRowElement *copy() const = 0;

  /**
   * @return the type of the attribute
   */
  virtual mrdb::Types::SQLType getType() const = 0;

  /**
   * Sets the value of the attribute accordingly to the value of the specified
   * column of the current row of the given result set
   *
   * @param rs the result set source of the value
   * @param col the index (starting from 1) of the column in the result set
   */
  virtual void setValue(mrdb::ResultSet *rs, int col) = 0;

  /**
   * Sets the value of the attribute according to the given string (converting
   * the value to the proper type when necessary).
   *
   * @param value a string representation of the value to be set
   */
  virtual void setValue(const std::string & value) = 0;

  /**
   * Compares the present attribute value with the one of the given attribute.
   *
   * @return -1 if the present attribute is less than the argument, 0 if they are
   * equal +1 otherwise
   */
  virtual int compareTo(const SourceRowAttribute &) const = 0;

  /**
   * Convert this attribute to a string.
   *
   * @param sep a separator string. Composite attributes should use this value
   *    to separate fields.
   * @return the std::string representation of the value of this attribute.
   */
  virtual std::string asString(const std::string &sep = ",") const = 0;

  // INHERITED FROM SOURCE ROW ELEMENT

  /// @return true iff s1<s2
  virtual bool operator()(const SourceRowElement &s1,
                          const SourceRowElement &s2) const {
    const SourceRowAttribute &attr1 = dynamic_cast<const SourceRowAttribute &>(s1);
    const SourceRowAttribute &attr2 = dynamic_cast<const SourceRowAttribute &>(s2);

    return attr1.compareTo(attr2) < 0;
  }

  /// @return true iff *this == s1
  virtual bool operator==(const SourceRowElement &s1) const {
    const SourceRowAttribute &attr1 = dynamic_cast<const SourceRowAttribute &>(s1);

    return this->compareTo(attr1) == 0;
  }

  /// return true if this attribute is empty
  virtual bool empty() const { return false; }

  /**
   * @return the std::string representation for the attribute in a format
   * suitable to be used in SQL queries (tipically it is implemented
   * as return "'"+this->asString()+"'"
   */
  virtual std::string getSQLData() const = 0;

  /**
   * Inserts the value of this attribute in the provided ostream and
   * returns the ostream.
   *
   * @param os an output stream
   * @return the output stream given in input
   */
  virtual std::ostream &operator<<(std::ostream &os) const {
    os << asString();
    return os;
  }
};

/**
 * This is a generic attribute class, i.e. it can contain any
 * attribute type. It should be used when more specific classes
 * are not available.
 */

class GenericSourceRowAttribute : public SourceRowAttribute {
  friend std::ostream &operator<<(std::ostream &os,const SourceRowAttribute &ia);

private:
  std::string value;
  mrdb::Types::SQLType type;

public:
  GenericSourceRowAttribute(mrdb::ResultSet *_rs, int _elem,
                            mrdb::Types::SQLType _type)
      : value(_rs->getString(_elem)), type(_type) {}

  GenericSourceRowAttribute()
      : value(""), type((mrdb::Types::SQLType)0)
        /*, colId(-1) */ {};

  // copy constructor
  GenericSourceRowAttribute(const GenericSourceRowAttribute &rhs)
      : value(rhs.value), type(rhs.type) /*, colId(rhs.colId)*/ {}

  virtual ~GenericSourceRowAttribute() {
    //      std::cout << "Generic... destructor" << std::endl;
  }

  // Implementing AttributeType methods...

  virtual SourceRowElement *copy() const {
    SourceRowAttribute *newRow = new GenericSourceRowAttribute(*this);
    return newRow;
  }

  virtual void setPreparedStatementParameters(mrdb::PreparedStatement *state,
                                              size_t start_index) const {
    state->setString(start_index, value);
  }

  virtual mrdb::Types::SQLType getType() const;


  virtual void setValue(mrdb::ResultSet *, int);
  virtual void setValue(const std::string &);
  virtual int compareTo(const SourceRowAttribute &) const;

  virtual std::string asString(const std::string &sep = ",") const;

  virtual std::string getSQLData() const;

  virtual SourceRowElement &operator=(const SourceRowElement &_rhs) {
    const GenericSourceRowAttribute &rhs =
        dynamic_cast<const GenericSourceRowAttribute &>(_rhs);
    value = rhs.value;
    type = rhs.type;
    return *this;
  }

  /**
   * Serialization methods
   */
  virtual ElementType getElementType() const { return 'g'; }
  virtual void serialize(std::ostream &os) const ;
  virtual void deserialize(std::istream &is) ;
};

/**
 * This class keep track of how many instances has been created. It is useful
 * in order to check if a program correctly deallocates sourcerowattribute instances.
 */

class MemDebugGenericSourceRowAttribute : public GenericSourceRowAttribute {
  static size_t instanceCount;

private:
  void incCounter() {
    instanceCount++;
  }

  void decCounter() {
    instanceCount--;
  }

public:
  MemDebugGenericSourceRowAttribute() : GenericSourceRowAttribute() {
    incCounter();
  }

  MemDebugGenericSourceRowAttribute(mrdb::ResultSet *_rs, int _elem,
                                    mrdb::Types::SQLType _type)
      : GenericSourceRowAttribute(_rs, _elem, _type) {
    incCounter();
  }

  MemDebugGenericSourceRowAttribute(
      const MemDebugGenericSourceRowAttribute &rhs)
      : GenericSourceRowAttribute(rhs) {
    incCounter();
  }

  virtual ~MemDebugGenericSourceRowAttribute() { decCounter(); }

  virtual SourceRowElement *copy() const {
    SourceRowAttribute *newRow = new MemDebugGenericSourceRowAttribute(*this);
    return newRow;
  }

  static size_t getInstanceCounter() { return instanceCount; }
};

/**
 * This class represents int valued source row attributes. Probably "numeric"
 * is a misnomer since it hints that the attribute can handle also real numbers.
 * It can't, only integers are supported.
 */
class NumericSourceRowAttribute : public SourceRowAttribute {
  friend std::ostream &operator<<(std::ostream &os,
                                  const SourceRowAttribute &ia);

private:
  long int value;

public:
  const static SourceRowAttribute::ElementType elementType;

  NumericSourceRowAttribute(mrdb::ResultSet *_rs, int _elem)
      : value(_rs->getInt(_elem)) {}

  NumericSourceRowAttribute() : value(0){};

  // copy constructor
  NumericSourceRowAttribute(const NumericSourceRowAttribute &rhs)
      : value(rhs.value) {}

  virtual ~NumericSourceRowAttribute() {
    //      std::cout << "Numeric... destructor" << std::endl;
  }

  // Implementing AttributeType methods...

  virtual SourceRowElement *copy() const {
    SourceRowAttribute *newRow = new NumericSourceRowAttribute(*this);
    return newRow;
  }

  virtual void setPreparedStatementParameters(mrdb::PreparedStatement *state,
                                              size_t start_index) const {
    state->setLong(start_index, value);
  }

  virtual mrdb::Types::SQLType getType() const { return mrdb::Types::INTEGER; }

  virtual void setValue(mrdb::ResultSet *, int);
  virtual void setValue(const std::string &);
  virtual int compareTo(const SourceRowAttribute &) const;

  virtual std::string asString(const std::string &sep = ",") const;

  virtual std::string getSQLData() const;

  virtual SourceRowElement &operator=(const SourceRowElement &_rhs) {
    const NumericSourceRowAttribute &rhs =
        dynamic_cast<const NumericSourceRowAttribute &>(_rhs);
    value = rhs.value;
    return *this;
  }

  /**
   * Serialization methods
   */
  virtual ElementType getElementType() const { return 'n'; }
  virtual void serialize(std::ostream &os) const ;
  virtual void deserialize(std::istream &is) ;
};

inline std::ostream &operator<<(std::ostream &os,
                                const SourceRowAttribute &attr) {
  attr.operator<<(os);
  return os;
}

} // minerule;

#endif
