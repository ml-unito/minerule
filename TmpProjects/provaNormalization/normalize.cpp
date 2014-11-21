#include "minerule/PredicateUtils/SimplePredAnalyzer.hpp"

using namespace std;
using namespace minerule;


int
main( int argc, char** argv ) {
  cout << SimplePredAnalyzer::getRelation(argv[1],argv[2],argv[3],argv[4], SQLUtils::String) << endl;
}
