#ifndef __CARE_H
#define __CARE_H

#include "Algorithms/MiningAlgorithmBase.h"
#include "Database/Connection.h"
#include "Algorithms/Bodymap.h"

namespace minerule {

  class CARE : public MiningAlgorithm {
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
    CARE(const OptimizedMinerule& mr) :
      MiningAlgorithm(mr), statement(NULL), stmt1(NULL) {}

    virtual ~CARE() {}

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

