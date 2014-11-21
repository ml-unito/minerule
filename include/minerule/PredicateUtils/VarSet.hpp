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
#ifndef __VARSET_H__
#define __VARSET_H__

#include <iostream>
#include <cassert>


/* ************************************************** 
 * In this file two classes are defined: 
 * - VarSet : allows to define and store  efficiently the truth value of a 
 *            set of boolean variables (limitation: the current implementation 
 *            only deals with variable  sets whose size is less than 32 elements) 
 * - VarSetEnumerator : allows to iterates through all possible subsets
 *            that can be formed starting from a VarSet of a given size.  
 *            It comes handy since the most frequent operation on a ExpressionEvaluator
 *            is to iterate through all possible subsets of a VarSet
 *            evaluating the expression * in the context provided by each subset. 
 * ************************************************** */	      


namespace minerule {

class VarSet {
  typedef unsigned int SetType;

  SetType vset;
  unsigned short vsetSize;

 public:
  VarSet(size_t numVars) :vset(0), vsetSize(numVars) {
    assert( vsetSize < sizeof(SetType)*8 );
  }

  VarSet() : vset(0), vsetSize(0) {

  }

  void setSize(size_t numVars) {
    assert( numVars < sizeof(SetType)*8 );
    vsetSize = numVars;
  }

  void setVar(size_t varNum, bool value) {
    assert(varNum < vsetSize);
    size_t mask = 1<<varNum;

    if(value) {
      vset |= mask;
    } else {
      vset &= ~mask;
    }
  }

  bool getVar(size_t varNum) const {
    assert(varNum < vsetSize);

    return (1<<varNum) & vset;
  }

  
  // This function is an helper function for VarSetEnumerator class.
  // It assumes that the present instance is used to enumerate all
  // possible subsets of a set having vsetSize elements. If the
  // current instance does NOT represent the last of the subsets that
  // can be enumerated it returns true and move to the next subset, otherwise
  // it return false.

  bool operator++(int) {
    if(vset+1 < (unsigned int)(1<<vsetSize)) {
      vset++;
      return true;
    } else
      return false;
  }

  VarSet& operator=(const VarSet& v) {
    vset=v.vset;
    vsetSize=v.vsetSize;
    return *this;
  }

  bool operator<(const VarSet& v) const {
    assert(v.vsetSize==vsetSize);
    return vset<v.vset;
  }

  bool operator==(const VarSet& v) const {
    assert(v.vsetSize==vsetSize);
    return vset==v.vset;
  }

  bool operator>(const VarSet& v) const {
    assert(v.vsetSize==vsetSize);
    return vset>v.vset;
  }

  size_t size() const {
    return vsetSize;
  }
};


class VarSetEnumerator {
  VarSet varset;
  bool firstCall;


 public:
  VarSetEnumerator(size_t numVars) : varset(numVars), firstCall(true) {

  }

  const VarSet& operator*() const {
    return varset;
  }

  bool operator++(int)  {
    if(firstCall && varset.size()!=0 ) { 
      firstCall=false;
      return true;
    }
    else
      return varset++;
  }

  // if a parameter val!=0 is provided, it returns the number of valid subsets
  // that can be formed from a set having size val. Otherwise it returns the same
  // information, but using the current VarSet size in evaluating it.
  size_t enumerationSize(size_t val=0) const {
    if( val==0 )
      val = varset.size();

    return (size_t)(1<<val);
  }
};


// streaming operators
inline std::ostream& operator<<(std::ostream& os, const VarSet& vset) {
  for(size_t i=0; i<vset.size(); i++) {
    if(vset.getVar(i))
      os << "1";
    else
      os << "0";
  }
  return os;
}


} // namespace minerule


#endif
