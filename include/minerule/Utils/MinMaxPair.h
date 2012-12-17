#ifndef __MINMAXPAIR_H__
#define __MINMAXPAIR_H__

#include <iostream>
#include <assert.h>

namespace minerule {

class MinMaxPair {
 private:
  int _min;
  int _max;

  static int _MIN(int m1, int m2) {
    if(m1<m2)
      return m1;
    else 
      return m2;
  }

  static int _MAX(int m1, int m2) {
    if(m1>m2)
      return m1;
    else
      return m2;
  }
  
 public:
  MinMaxPair(int m,int M) : _min(m), _max(M) {
    assert( _min <= _max );
  };

  ~MinMaxPair() {};

  void applyConstraints(const MinMaxPair& constr) {
    assert( _min<=_max );
    assert( constr._min <= constr._max );

    // the minimal el, cannot be lower than constr.minimal so we set
    // it to the maximum of the two
    _min = _MAX( _min, constr._min );
    // at the same time it cannot be higher than constr.maximal so we
    // set it to the minimum of the two
    _min = _MIN( _min, constr._max );
    
    // An analogous reasoning lead to the following two lines of code
    _max = _MIN( _max, constr._max );
    _max = _MAX( _max, constr._min );

    assert( _min<=_max );
  }

  void setMin(int m) {
    _min=m;
  }

  void setMax(int M) {
    _max=M;
  }

  int getMin() const {
    return _min;
  }

  int getMax() const {
    return _max;
  }

  int getDefaultMax() const {
    return 1000;
  }

  void setDefaultMax() {
    setMax(getDefaultMax());
  }

  bool validate( int value ) const {
    if( _min <= value && value <= _max )
      return true;
    else
      return false;
  }

  bool contains(const MinMaxPair& mm) const {
    return _min<=mm._min && mm._max<=_max;
  }
};

 inline std::ostream& operator<<(std::ostream& os, const MinMaxPair& p) {
   os << "MinMax(" << p.getMin() << "," << p.getMax() << ")";
   return os;
 }
} // namespace



#endif
