#ifndef __FSMINER_H__
#define __FSMINER_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/sqlCoreConn.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {

  class FSMiner : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;

    sqlCoreConn coreConn;
    HeadBodySourceRowDescription rowDes;
    odbc::PreparedStatement* statement;

    void prepareData();
    
  public:
    FSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) : 
       MiningAlgorithm(mr), options(opts), statement(NULL) {}

    virtual ~FSMiner() {}

    virtual void execute();

    virtual bool canHandleMinerule() const {
      return minerule.getParsedMinerule().miningTask == MTMineSequences;
    }

    virtual bool needsGidSortedSourceTable() const {
      return true;
    }


  };
}



#endif
