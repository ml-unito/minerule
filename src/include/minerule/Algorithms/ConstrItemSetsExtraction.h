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
    HeadBodySourceRowDescription rowDes;
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

