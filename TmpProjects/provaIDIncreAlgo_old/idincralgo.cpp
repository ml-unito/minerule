#include "Algorithms/IDIncrementalAlgorithm.h"
#include "Utils/MineruleOptions.h"

using namespace minerule;
using namespace std;

int
main(int argc, char** argv) {
  try {
    string oldMRstr =
      "MINE RULE mrArianna5 AS SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.012,CONFIDENCE:0.96";

    string newMRstr =
      "MINE RULE idincr2 AS SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE WHERE (BODY.descCategory<'c' AND BODY.descCategory>'a' AND HEAD.descCategory<'c' AND HEAD.descCategory>'a') FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.012,CONFIDENCE:0.96";


    MineruleOptions& mropt =
      MineruleOptions::getSharedOptions();

    mropt.readFromFile("mroptions.txt");

    MRLogPush("Starting...");

    ParsedMinerule oldMinerule(oldMRstr);
    OptimizedMinerule mr(newMRstr);

    mr.getOptimizationInfo().relationship = OptimizedMinerule::Dominance;
    mr.getOptimizationInfo().minerule = oldMinerule;

    MRLogPush("IDIncrementalAlgorithm started...");
    IDIncrementalAlgorithm incr(mr);
    incr.execute();
    MRLogPop();

    MRLogPop();
  } catch (std::exception& e) {
    cout << e.what() << endl;
  }

  return 0;
}
