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
#ifndef __EXPRESSIONNFCODER_H__
#define __EXPRESSIONNFCODER_H__

#include "minerule/Utils/MineruleException.hpp"
#include "minerule/PredicateUtils/Predicate.hpp"
#include "minerule/PredicateUtils/InvalidConfigurationFilter.hpp"

// This class provide facilities to encode/decode the normal form
// of an Expression contained in an Expression Evaluator

namespace minerule {

  typedef unsigned int EncodingBaseType;

  // ----------------------------------------------------------------------
  // EncodedNF
  // ----------------------------------------------------------------------

  class EncodedNF {
  public:
     typedef enum {
      FirstMoreGeneral,
      FirstMoreSpecific,
      Equivalent,
      Unrelated
    } CodesRelationship;

    const EncodingBaseType* encVector;
    size_t numVars;    // num of variables present in the expression that
    // originated this EncodedNF
    size_t vecSize;    // the size of encVector
    size_t codingLen;  // the exact number of elements that were encoded

    bool operator==(const EncodedNF&) const;

    static CodesRelationship
      getCodesRelationship( const EncodedNF&, const EncodedNF& );

    
    static size_t 
      computeHammingDistance(const EncodedNF& nf1, 
			     const EncodedNF& nf2);
  };

  std::ostream& operator<<(std::ostream& os, const EncodedNF&);

  // ----------------------------------------------------------------------
  // EncodedNFIterator
  // ----------------------------------------------------------------------

  class EncodedNFIterator {
    const EncodedNF& target;
    VarSet currentVarSet;
    size_t cellCounter;
    size_t elemCounter;
    bool itok;
  public:
    EncodedNFIterator(const EncodedNF& er): 
      target(er), currentVarSet(er.numVars), cellCounter(0), elemCounter(0), itok(false) {

    }

    bool ok() const { 
      return itok;
    }

    void setOk(bool val) {
      itok=val;
    }

    bool operator++(int);

    const VarSet& operator*() const {
      return currentVarSet;
    }
  };

  // ----------------------------------------------------------------------
  // ExpressionNFCoder
  // ----------------------------------------------------------------------

  class ExpressionNFCoder {
  protected:
    static const size_t bitsPerCell;
    InvalidConfigurationFilter& filter;

  private:
    EncodingBaseType* buf;

    void
      cleanBuf() {
      if(buf!=NULL)
	free(buf);
      buf=NULL;
    }
  public:
    ExpressionNFCoder(InvalidConfigurationFilter& f):  filter(f), buf(NULL) {

    }

    ~ExpressionNFCoder() {
      cleanBuf();
    }

    // Return the encoded representation for the given expression normal
    // form. The returned pointer will be deleted as soon a new encoding
    // is requested or upon object deallocation.
    EncodedNF
      encode(Predicate&) throw(MineruleException);
  };

} // namespace minerule

#endif
