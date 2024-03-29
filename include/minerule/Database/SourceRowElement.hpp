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
#ifndef __SOURCEROWELEMENT_H__
#define __SOURCEROWELEMENT_H__

#include <vector>
#include <iostream>
#include <cassert>
#include <string.h>
#include "minerule/mrdb/ResultSetMetaData.hpp"
#include "minerule/mrdb/ResultSet.hpp"
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/mrdb/PreparedStatement.hpp"

namespace minerule {

/**
 * Base class for all source row attributes and attributes collection.
 */

class SourceRowElement {

public:
  typedef char ElementType;

  static SourceRowElement *createElement(mrdb::ResultSetMetaData *rsmd,
                                         mrdb::ResultSet *rs,
                                         const std::vector<int> &srd);

  virtual ~SourceRowElement() {}

  virtual SourceRowElement *copy() const = 0;

  // Returns true if s1<s2
  virtual bool operator()(const SourceRowElement &s1,
                          const SourceRowElement &s2) const = 0;

  virtual bool operator==(const SourceRowElement &) const = 0;

  virtual bool operator!=(const SourceRowElement &el) const {
    return !this->operator==(el);
  }

  virtual bool operator<(const SourceRowElement &el) const {
    return this->operator()(*this, el);
  }

  //    const CollectionType& getConstAttributes() const=0;

  virtual SourceRowElement &operator=(const SourceRowElement &rhs) = 0;

  virtual bool empty() const = 0;

  virtual std::string asString(const std::string &sep = ",") const = 0;

  virtual void setPreparedStatementParameters(mrdb::PreparedStatement *state,
                                              size_t start_index) const = 0;

  // This function should return a std::string having the sql
  // representation of the data in the object (it is used
  // by some algorithm to store temporary information about
  // the data.

  virtual std::string getSQLData() const = 0;

  static SourceRowElement *
  createElementFromType(ElementType el) ;

  virtual ElementType getElementType() const {
    throw MineruleException(MR_ERROR_INTERNAL,
                            "getElementType() not implemented!");
  }

  // Returns a std::string description of the type of this element and, in case
  // it applies,
  // of inner elments. Defaults to a std::string representation of
  // getElementType() except
  // when the element is composed.
  virtual std::string getFullElementType() const {
    char chstr[2] = {getElementType(), '\0'};
    return std::string(chstr);
  }

  virtual void serialize(std::ostream &os) const = 0;
  virtual void deserialize(std::istream &is) = 0;

  static SourceRowElement *deserializeElementFromString(
      const std::string &strRepr) ;

  static SourceRowElement *
  deserializeElementFromResultSet(mrdb::ResultSet *rs,
                                  size_t start_index) ;

  static void
  serializeElementToString(const SourceRowElement &elem,
                           std::string &strRepr) ;

  virtual std::ostream &operator<<(std::ostream &) const = 0;
};

class SourceRowEmptyElement : public SourceRowElement {

public:
  SourceRowEmptyElement() {}

  virtual ~SourceRowEmptyElement() {}

  virtual SourceRowElement *copy() const { return new SourceRowEmptyElement(); }

  virtual void setPreparedStatementParameters(mrdb::PreparedStatement *state,
                                              size_t start_index) const {
    throw MineruleException(MR_ERROR_INTERNAL,
                            "setPreparedStatementParameters callled on an "
                            "empty element. This is a bug! Please report it!");
  }

  // Returns true if s1<s2
  virtual bool operator()(const SourceRowElement &s1,
                          const SourceRowElement &s2) const {
    const SourceRowEmptyElement *e1 =
        dynamic_cast<const SourceRowEmptyElement *>(&s1);

    const SourceRowEmptyElement *e2 =
        dynamic_cast<const SourceRowEmptyElement *>(&s2);

    // if both s1 and s2 are NOT emptyElements, then the
    // proper function should be called
    if (e1 == NULL && e2 == NULL) {
      return s1(s1, s2);
      ;
    }

    // an empty element is always "lesser" than any non
    // empty one
    if (e1 == NULL)
      return true;

    // if s1 is empty and s2 is not than s1<s2 is false
    // if both are empty then s1==s2 (hence s1<s2 is false)
    // in any case we have to return false
    return false;
  }

  virtual bool operator==(const SourceRowElement &s1) const {
    const SourceRowEmptyElement *e1 =
        dynamic_cast<const SourceRowEmptyElement *>(&s1);

    return e1 != NULL;
  }

  //    const CollectionType& getConstAttributes() const=0;

  virtual SourceRowElement &operator=(const SourceRowElement &rhs) {
    assert(dynamic_cast<const SourceRowEmptyElement *>(&rhs) != NULL);

    return *this;
  }

  virtual bool empty() const { return false; }

  virtual std::string asString(const std::string &sep = ",") const {
    return "";
  }

  virtual std::string getSQLData() const { assert(false); }

  virtual std::ostream &operator<<(std::ostream &os) const {
    os << "(emptyElem)";
    return os;
  }

  virtual ElementType getElementType() const { return '0'; }
  virtual void serialize(std::ostream &os) const {
    os << " NULL";
  }
  virtual void deserialize(std::istream &is) {
    std::string null;
    is >> null;
    if (null != " NULL")
      throw MineruleException(MR_ERROR_INTERNAL,
                              "NULL Element expected, but " + null + " found!");
  }
};

inline std::ostream &operator<<(std::ostream &os, const SourceRowElement &el) {
  el.operator<<(os);
  return os;
}

} // namespace

#endif
