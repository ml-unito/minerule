#ifndef __FSMINER_H__
#define __FSMINER_H__

#include "Algorithms/MiningAlgorithmBase.h"
#include "Database/Connection.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {

  class FSMiner : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;

    Connection connection;
    SourceRowColumnIds rowDes;
    odbc::PreparedStatement* statement;

    void prepareData();
    
  public:
    FSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) : 
       MiningAlgorithm(mr), options(opts), statement(NULL) {}

    virtual ~FSMiner() {}

    virtual void mineRules();

    virtual bool canHandleMinerule() const {
      return minerule.getParsedMinerule().miningTask == MTMineSequences;
    }

		virtual SourceTableRequirements sourceTableRequirements() const {
			return SourceTableRequirements(SourceTableRequirements::SortedGids);
		};


  };
}



#endif