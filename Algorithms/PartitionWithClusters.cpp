#include "PartitionWithClusters.h"
#include "AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h" // for MRLog()

void partitionWithClusters(const minerule::AlgorithmsOptions& algoOptions);

namespace minerule {


void PartitionWithClusters::execute() {
  MRLog() << "This is Partition (with clusters) mining algorithm..." << endl;
  prepareData();
  partitionWithClusters(options);
};

}

