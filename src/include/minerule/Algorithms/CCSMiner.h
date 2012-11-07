#ifndef __CCSMINER_H__
#define __CCSMINER_H__

#include "Algorithms/CCSMSequence.h"
#include "Algorithms/MiningAlgorithmBase.h"
#include "Algorithms/sqlCoreConn.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {

  class CCSMiner : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    sqlCoreConn coreConn;
    HeadBodySourceRowDescription rowDes;
    odbc::PreparedStatement* statement;

    void prepareData();
    
  public:
    CCSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) : 
      MiningAlgorithm(mr), options(opts), statement(NULL) {}

    virtual ~CCSMiner() {}

    virtual void execute();

    virtual bool canHandleMinerule() const {
      return minerule.getParsedMinerule().miningTask == MTMineSequences;
    }

    virtual bool needsGidSortedSourceTable() const {
      return true;
    }

    bool find(std::vector<CCSMSequence*>*vec, CCSMSequence* elem); 

    void combina(std::vector<CCSMSequence::ResultItems>&, std::vector<CCSMSequence*>* k2, size_t k, int min_g, int max_g, double threshold, int check);

  };
}



#endif
