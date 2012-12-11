#ifndef __RESULT_COMBINATOR_H__
#define __RESULT_COMBINATOR_H__

#include "Algorithms/IncrementalAlgorithm.h"

namespace minerule {

  class ResultCombinator : public IncrementalAlgorithm {
  public:
    ResultCombinator(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr) {}
    virtual ~ResultCombinator() {}

    virtual void execute() throw(MineruleException,odbc::SQLException);
  };

}

#endif
