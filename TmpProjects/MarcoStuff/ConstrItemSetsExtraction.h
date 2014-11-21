#ifndef __CONSTRITEMSETSEXTRACTION_H
#define __CONSTRITEMSETSEXTRACTION_H

#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/Connection.hpp"
//#include "bodymap.h"
#include "minerule/Algorithms/Bodymap.hpp"

namespace minerule {

  class ConstrItemSetsExtraction : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    Connection connection;
    SourceRowColumnIds rowDes;
    odbc::PreparedStatement* statement;
    odbc::PreparedStatement* stmt1;



    size_t buildAttrStr(const ParsedMinerule::AttrVector& attr,
                        size_t startIndex,
                        std::string& attrStr,
                        std::vector<int>& des) const;

    std::string buildQry( const std::string& groupAttrStr,
                          const std::string& attrStr,
                          const std::string& constraints) const;

    void prepareData();

  public:
    ConstrItemSetsExtraction(const OptimizedMinerule& mr) :
      MiningAlgorithm(mr), statement(NULL), stmt1(NULL) {}

    virtual ~ConstrItemSetsExtraction() {}

    virtual void execute();

    virtual bool canHandleMinerule() const {
      return
        !minerule.getParsedMinerule().hasCrossConditions() &&
        !minerule.getParsedMinerule().requiresClusters() &&
        !minerule.getParsedMinerule().hasDisjuctionsInMC();
    }

  };
}



#endif
