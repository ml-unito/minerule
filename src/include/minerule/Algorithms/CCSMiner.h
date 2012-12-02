#ifndef __CCSMINER_H__
#define __CCSMINER_H__

#include "Algorithms/CCSMSequence.h"
#include "Algorithms/MiningAlgorithmBase.h"
#include "Database/Connection.h"
#include "Utils/AlgorithmTypes.h"

namespace minerule {

  class CCSMiner : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    Connection connection;
    SourceRowColumnIds rowDes;
    odbc::PreparedStatement* statement;

    void prepareData();
    
  public:
    CCSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) : 
      MiningAlgorithm(mr), options(opts), statement(NULL) {}

    virtual ~CCSMiner() {}

    virtual void mineRules();

    virtual bool canHandleMinerule() const {
      return minerule.getParsedMinerule().miningTask == MTMineSequences;
    }

		virtual SourceTableRequirements sourceTableRequirements() const {
			return SourceTableRequirements(SourceTableRequirements::SortedGids);
		};

    bool find(std::vector<CCSMSequence*>*vec, CCSMSequence* elem); 

    void combina(std::vector<CCSMSequence::ResultItems>&, std::vector<CCSMSequence*>* k2, size_t k, int min_g, int max_g, double threshold, int check);

  };
}



#endif
