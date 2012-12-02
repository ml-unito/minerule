#ifndef __CONSTRITEMSETSEXTRACTION_H
#define __CONSTRiTEMSETSEXTRACTION_H

#include "Algorithms/MiningAlgorithmBase.h"
#include "Database/Connection.h"
#include "Algorithms/Bodymap.h"

namespace minerule {

  class ConstrItemSetsExtraction : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    Connection connection;
	SourceTable* sourceTable;
	SourceTable::Iterator bodyIterator;


    void prepareData();

  public:
    ConstrItemSetsExtraction(const OptimizedMinerule& mr) :
      MiningAlgorithm(mr) {}

    virtual ~ConstrItemSetsExtraction() {}

    virtual void mineRules();

    virtual bool canHandleMinerule() const {
      return
        !minerule.getParsedMinerule().hasCrossConditions() &&
        !minerule.getParsedMinerule().requiresClusters() &&
        !minerule.getParsedMinerule().hasDisjuctionsInMC();
    }

  };
}



#endif

