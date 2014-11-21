#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "minerule/Utils/MineruleException.hpp"
#include "minerule/Utils/Converter.hpp"
#include <odbc++/drivermanager.h>
#include <odbc++/statement.h>
#include <odbc++/connection.h>
#include <odbc++/resultset.h>

/**
 * This file contains a reimplementation of the MRDatabase framework. It should
 * improve the overall execution times by implementing in a C like style the 
 * functionality that were demanded to the c++ run time. In particular there will
 * be a single object which represents an item attribute. The logic needed to 
 * distinguish among different types is implemented internally to the object.
 * This allows one to avoid the use of virtual functions and let us gain in
 * efficiency. 
 */


class ItemAttr {
public:
  typedef enum {
    Int,
    String,
    Double,
    None
  } Type;

  typedef union {
    int intVal;
    double doubleVal;
    const char* stringVal;
  } AllTypes;

  AllTypes value;
private:
  Type type;

public:
  ItemAttr() : type(None) { value.intVal=0;}
  explicit ItemAttr(int ival) :  type(Int) { value.intVal=ival; }
  explicit ItemAttr(double dval) : type(Double) { value.doubleVal=dval; }
  explicit ItemAttr(const char* str) : type(String) { value.stringVal=str; }

  ItemAttr(const ItemAttr& rhs) : type(rhs.type), value(rhs.value) {}
  ItemAttr& operator=(const ItemAttr& rhs) {
    type=rhs.type;
    value=rhs.value;
    return *this;
  }


  AllTypes getValue() const {
    return value;
  }

  Type getType() const {
    return type;
  }

  bool operator<(const ItemAttr& a) const {
    assert(a.type==type);
    switch(type) {
    case None:
      return false;
    case Int:
      return value.intVal < a.value.intVal;
    case Double:
      return value.doubleVal < a.value.doubleVal;
    case String:
      return strcmp(value.stringVal, a.value.stringVal)<0;
    default:
      assert(false);
    }
  }
  
  bool operator>(const ItemAttr& a) const {
    assert(a.type==type);
    switch(type) {
    case None:
      return false;
    case Int:
      return value.intVal > a.value.intVal;
    case Double:
      return value.doubleVal > a.value.doubleVal;
    case String:
      return strcmp(value.stringVal, a.value.stringVal)>0;
    default:
      assert(false);
    }
  }

   bool operator==(const ItemAttr& a) const {
    assert(a.type==type);
    switch(type) {
    case None:
      return false;
    case Int:
      return value.intVal == a.value.intVal;
    case Double:
      return value.doubleVal == a.value.doubleVal;
    case String:
      return strcmp(value.stringVal, a.value.stringVal)==0;
    default:
      assert(false);
    }
  }


  bool operator!=(const ItemAttr& a) const {
    assert(a.type==type);
    switch(type) {
    case None:
      return false;
    case Int:
      return value.intVal != a.value.intVal;
    case Double:
      return value.doubleVal != a.value.doubleVal;
    case String:
      return strcmp(value.stringVal, a.value.stringVal)!=0;
    default:
      assert(false);
    }
  }

  std::string
  getSQLData() const {
    assert(type!=None);
    switch(type) {
    case Int:
      return "'"+minerule::Converter(value.intVal).toString()+"'";
    case Double:
      return "'"+minerule::Converter(value.doubleVal).toString()+"'";
    case String:
      return "'"+std::string(value.stringVal)+"'";
    default:
      assert(false);
    }
  }
};


class ItemAttrPtrSmaller {
public:
  bool operator()(const ItemAttr* a1, const ItemAttr* a2) {
    return *a1 < *a2;
  }
};

class ItemAttrPtrLarger {
public:
  bool operator()(const ItemAttr* a1, const ItemAttr* a2) {
    return *a1 > *a2;
  }
};


class ItemVec {
  std::vector<ItemAttr*> vec;
public:
  ItemVec() {}
  ItemVec(const ItemVec& rhs) : vec(rhs.vec) {}
  
  ItemVec& operator=(const ItemVec& rhs) {
    vec=rhs.vec;
  }

  void addAttr(ItemAttr* attr) {
    vec.push_back(attr);
  }

  bool operator<(const ItemVec& v) const {
    return lexicographical_compare(vec.begin(),
				   vec.end(),
				   v.vec.begin(),
				   v.vec.end(),
				   ItemAttrPtrSmaller());
  }

  bool operator==(const ItemVec& v) const {
    return vec.size()==v.vec.size() &&
      equal( vec.begin(), vec.end(), v.vec.begin() );
  }

  bool operator>(const ItemVec& v) const {
    return lexicographical_compare(vec.begin(),
				   vec.end(),
				   v.vec.begin(),
				   v.vec.end(),
				   ItemAttrPtrLarger());
  }

  bool operator!=(const ItemVec& v) const {
    return !operator==(v);
  }
};

class ItemType {
  const ItemVec* vec;
  ItemAttr attr;
public:
  ItemType( const ItemAttr& a) : vec(NULL), attr(a) {}
  ItemType( const ItemVec* v) : vec(v), attr() {}
  
  bool operator<(const ItemType& rhs) const {
    if(vec!=NULL) {
      assert(rhs.vec!=NULL);
      return *vec < *rhs.vec;
    } else
      return attr < rhs.attr;
  }

  bool operator>(const ItemType& rhs) const {
    if(vec!=NULL) {
      assert(rhs.vec!=NULL);
      return *vec > *rhs.vec;
    } else
      return attr > rhs.attr;
  }
  
  bool operator==(const ItemType& rhs) const {
    if(vec!=NULL) {
      assert(rhs.vec!=NULL);
      return *vec == *rhs.vec;
    } else
      return attr == rhs.attr;
  }
  
  bool operator!=(const ItemType& rhs) const {
    if(vec!=NULL) {
      assert(rhs.vec!=NULL);
      return *vec > *rhs.vec;
    } else
      return attr > rhs.attr;
  }
};

