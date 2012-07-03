#ifndef __BFSWITHGIDSNOCROSS_H__
#define __BFSWITHGIDSNOCROSS_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/sqlCoreConn.h"
#include "bodymap.h"
//#include "Algorithms/bodymap.h"

namespace minerule {

  class CARE : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    sqlCoreConn coreConn;
    HeadBodySourceRowDescription rowDes;
    odbc::PreparedStatement* statement;
    odbc::PreparedStatement* stmt1;



    size_t buildAttrStr(const ParsedMinerule::ListType& attr,
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

