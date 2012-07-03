#include "Algorithms/IDIncrementalAlgorithm.h"
#include "Utils/MineruleOptions.h"
#include <iterator>

using namespace minerule;
using namespace std;

struct ResultRow {
  string qryname;
  double secs;
};

ostream& operator<<(ostream& os, const struct ResultRow& rr) {
  cout << rr.qryname << " " << rr.secs;
}

double
execAlgorithm( std::string newqryname,
	       std::string constraint ) throw (odbc::SQLException) {
  string oldMRstr =
    "MINE RULE mrArianna5 AS SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.012,CONFIDENCE:0.96";
  
  string newMRstr =
    "MINE RULE "+newqryname+" AS SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE WHERE "+constraint+" FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.012,CONFIDENCE:0.96";
  double secs;
  
  
  MRLogPush("Starting...");
  
  ParsedMinerule oldMinerule(oldMRstr);
  OptimizedMinerule mr(newMRstr);
  
  mr.getOptimizationInfo().relationship = OptimizedMinerule::Dominance;
  mr.getOptimizationInfo().minerule = oldMinerule;
  
  MRLogPush("IDIncrementalAlgorithm started...");
  IDIncrementalAlgorithm incr(mr);
  incr.execute();
  secs=MineruleOptions::getSharedOptions().getLogStreamObj().getLogger().getCurrentTimeDelta();
  MRLogPop();

  MRLogPop();
  return secs;
}



int
main(int argc, char** argv) {
  try {
    MineruleOptions& mropt =
      MineruleOptions::getSharedOptions();
    mropt.readFromFile("mroptions.txt");
  

    unsigned char ch;
    vector<struct ResultRow> results;
    for(ch='a';ch<'z'+1;ch++) {
      char buf[2];
      buf[0]=ch;
      buf[1]='\0';

      ResultRow rr;
      rr.qryname = string("idincr_")+buf;
      rr.secs =
	execAlgorithm( rr.qryname,
		       string("(BODY.descCategory>'")+buf+"' AND HEAD.descCategory>'"+buf+"')");
      results.push_back(rr);
    }

    copy( results.begin(),
	  results.end(),
	  ostream_iterator< ResultRow >( cout, "\n" ) );
  } catch (std::exception& e) {
    cout << e.what() << endl;
  }

  return 0;
}
