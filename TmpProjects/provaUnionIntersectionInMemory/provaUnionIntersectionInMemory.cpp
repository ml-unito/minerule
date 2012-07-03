#include "Algorithms/ResultCombinator.h"
#include "Optimizer/OptimizedMinerule.h"
#include "Utils/MineruleOptions.h"


using namespace std;
using namespace minerule;


// const string TAB_SOURCE = "mrProvaUnionIntersectionInMemory";
const string TAB_SOURCE = "mrHugeQuery";
const string TMP_TAB1 = "tmp_table1";
const string TMP_TAB2 = "tmp_table2";

string
build_source(MineruleOptions& opts, string perc_string, string tab_name) {
  MRLogPusher log("Creating tmp table "+tab_name+" filtering out 1-"+perc_string+" of table "+TAB_SOURCE);
  odbc::Connection* connection = opts.getOdbc_db().getConnection();
  string query =
    "CREATE TABLE "+tab_name+" SELECT * FROM "+TAB_SOURCE+" WHERE RAND()<"+perc_string;
  string query_elems =
    "CREATE TABLE "+tab_name+"_elems SELECT * FROM "+TAB_SOURCE+"_elems";
  string query_add_index =
    "CREATE INDEX "+tab_name+"_elems_index ON "+tab_name+"_elems (id)";

  odbc::Statement* stmt =
    connection->createStatement();

  { MRLogPusher log2("Creating main table");  
    MRLog() << "Query String:" << query << endl;
    stmt->execute(query); }
  { MRLogPusher log3("Creating elems table"); 
    MRLog() << "Query String:" << query_elems << endl;
    stmt->execute(query_elems); }
  { MRLogPusher log3("Creating index on id field of elems table"); 
    MRLog() << "Query String:" << query_add_index << endl;
    stmt->execute(query_add_index); }

  delete stmt;

  return tab_name;
}

void
cleanUp(MineruleOptions& opts) {
  odbc::Connection* connection = opts.getOdbc_db().getConnection();
  odbc::Statement* stmt=connection->createStatement();
  stmt->execute("DROP TABLE IF EXISTS "+TAB_SOURCE+"Result");
  stmt->execute("DROP TABLE IF EXISTS "+TAB_SOURCE+"Result_elems");
  stmt->execute("DROP TABLE IF EXISTS "+TMP_TAB1);
  stmt->execute("DROP TABLE IF EXISTS "+TMP_TAB1+"_elems");
  stmt->execute("DROP TABLE IF EXISTS "+TMP_TAB2);
  stmt->execute("DROP TABLE IF EXISTS "+TMP_TAB2+"_elems");
  delete stmt;
}


int
main(int argc, char** argv) {
  try {
    if(argc<4)
      throw MineruleException(MR_ERROR_OPTION_PARSING,"you must provide at least 4 parameters.");

    MRLogPusher log("Starting testing program");
    MineruleOptions& mrOpts =  MineruleOptions::getSharedOptions();
    mrOpts.readFromFile("options.opt");

    
    cleanUp(mrOpts);

    OptimizedMinerule mr("MINE RULE "+ TAB_SOURCE +"Result AS "
			 "SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE "
			 "FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.0085,CONFIDENCE:1e-04");
    ParsedMinerule mr_to_combine_1;
    ParsedMinerule mr_to_combine_2;

#warning calcolare in qualche modo i due tab_result
    mr_to_combine_1.tab_result=build_source(mrOpts, argv[2], TMP_TAB1);
    mr_to_combine_2.tab_result=build_source(mrOpts, argv[3], TMP_TAB2);

    mr.getOptimizationInfo().relationship=OptimizedMinerule::Combination;
    mr.getOptimizationInfo().minerulesToCombine.push_back(mr_to_combine_1);
    mr.getOptimizationInfo().minerulesToCombine.push_back(mr_to_combine_2);


    if(argv[1][0]=='i') {
      vector<size_t> andFormula;
      andFormula.push_back(0);
      andFormula.push_back(1);
      mr.getOptimizationInfo().combinationFormula.push_back(andFormula);
    } else if(argv[1][0]=='u') {
      vector<size_t> andFormula1;
      vector<size_t> andFormula2;
      andFormula1.push_back(0);
      andFormula2.push_back(1);
      mr.getOptimizationInfo().combinationFormula.push_back(andFormula1);
      mr.getOptimizationInfo().combinationFormula.push_back(andFormula2);
    } else throw MineruleException(MR_ERROR_OPTION_PARSING,"argv[1][0] must be either 'i' or 'u'");

    ResultCombinator rc(mr);
    rc.execute();
  } catch (std::exception& e) {
    cout << "Exception thrown" << endl
	 << "The reason is:" << endl
	 << e.what() << endl;
    exit(1);
  }

  return 0;
}
