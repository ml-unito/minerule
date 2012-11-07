#include "Algorithms/PartitionWithClusters.h"
#include "Algorithms/AlgorithmsOptions.h"
#include "Utils/MineruleOptions.h" // for MRLog()

void partitionWithClusters(const minerule::AlgorithmsOptions& algoOptions);

namespace minerule {


void PartitionWithClusters::execute() {
  MRLog() << "This is Partition (with clusters) mining algorithm..." << std::endl;
  prepareData();
  partitionWithClusters(options);
};

}

