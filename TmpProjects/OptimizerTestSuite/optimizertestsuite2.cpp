/************************************************************************/
/* This program tests the performances of the Optimizer when the number */
/* of queries inside the catalogue increases.                           */
/* Note: it will fill the catalogue of the used DB with a lot of garbage*/
/*  it is hence better to use it on a dummy DB                          */
/************************************************************************/

#include <iostream>
#include "PredicateUtils/PredicateUtils.h"
#include "Utils/Converter.h"
#include "Utils/MineruleOptions.h"
#include "Optimizer/OptimizedMinerule.h"

using namespace minerule;

const char* MINERULE_TEMPLATE = 
"MINE RULE fakeMinerule%d AS "
"SELECT DISTINCT  1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE "
"WHERE BODY.price<10000 AND HEAD.price<10000 "
"FROM Sales "
"GROUP BY gid "
"EXTRACTING RULES WITH SUPPORT:%f, CONFIDENCE:%f";

// Hugely overestimate buffer size ;-)
char mineruletext[1024]; // the minerule text which will be used...


int
main() {
  MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
  mrOpts.getOdbc_db().setOption("name","bbdemo");
  mrOpts.getOdbc_db().setOption("username","root");
  mrOpts.getOdbc_db().setOption("password","mysql");
  mrOpts.getOdbc_db().resetConnection();
  mrOpts.getLogStreamObj().setOption("stream","/dev/null");
  mrOpts.getErrStreamObj().setOption("stream","/dev/null");
  mrOpts.getWarnStreamObj().setOption("stream","/dev/null");
  mrOpts.getDebugStreamObj().setOption("stream","/dev/null");

  mrOpts.getOptimizations().setOption("enableOptimizations","True");

  float support = 1.0;
  float confidence = 1.0;
  
  ofstream outlog("optTS_log.txt");
  ofstream outtimes("optTS_times.txt");
  assert(outlog && outtimes );

  MRLogger logger(outlog);
  outtimes << "# itnum cpu d-cpu time d-time" << endl;


  try {
    size_t itnum=0;

    while( confidence>0 ) {
      itnum++;
      sprintf(mineruletext, MINERULE_TEMPLATE, itnum, support, confidence);
      OptimizedMinerule optMr(mineruletext);
      
      logger.push("Executing minerule (itnum:"+Converter((int)itnum).toString()+")");
      optMr.optimize();
      OptimizerCatalogue::MineruleResultInfo result(optMr.getParsedMinerule());
      assert( optMr.getOptimizationInfo().relationship == OptimizedMinerule::None );
      result.resultset = optMr.getOptimizationInfo().minerule.tab_result;
      OptimizerCatalogue::addMineruleResult(result);
      
      
      outtimes << itnum << " "
	       << logger.getCurrentCpuSecs() << " "
	       << logger.getCurrentCpuDelta() << " " 
	       << logger.getCurrentTimeSecs() << " "
	       << logger.getCurrentTimeDelta() << endl;
      logger.pop();
      
      confidence-=0.01; // 100 iterations
    }
  } catch (std::exception& e) {
    cout << e.what() << endl;
    exit(1);
  }

}
