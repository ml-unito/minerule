#ifndef __CARE_H
#define __CARE_H

#include "minerule/Algorithms/MiningAlgorithmBase.h"
#include "minerule/Database/Connection.h"
#include "minerule/Algorithms/Bodymap.h"

namespace minerule {

  class CARE : public MiningAlgorithm {
  private:
	SourceTable* sourceTable;
	SourceTable::Iterator bodyIterator;
	SourceTable::Iterator headIterator;


    size_t buildAttrStr(const ParsedMinerule::AttrVector& attr,
                        size_t startIndex,
                        std::string& attrStr,
                        std::vector<int>& des) const;

    std::string buildQry( const std::string& groupAttrStr,
                          const std::string& attrStr,
                          const std::string& constraints) const;

    void prepareData();

  public:
    CARE(const OptimizedMinerule& mr) : MiningAlgorithm(mr) {}

    virtual ~CARE() {}

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

