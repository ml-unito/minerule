#include "Optimizer/OptimizerCatalogue.h"
#include "Utils/MineruleOptions.h"
#include <iterator>

using namespace minerule;
using namespace std;

int
main() {
  try {
    MineruleOptions& mropt
      = MineruleOptions::getSharedOptions();
  
    mropt.readFromFile("mroptions.txt");
    
    /*OptimizerCatalogue& optCat =
      mropt.getOptimizations().getCatalogue();

    const OptimizerCatalogue::Catalogue& dfcat =
      optCat.getCatalogue(OptimizerCatalogue::DEPFUN);

    OptimizerCatalogue::Catalogue::const_iterator it;
    for(it=dfcat.begin(); it!=dfcat.end(); it++) {
      cout << "Table:" << it->first << endl;
      OptimizerCatalogue::CatalogueEntry::const_iterator it2;
      for(it2=it->second.begin(); it2!=it->second.end(); it2++) {
	copy( it2->first.begin(),
	      it2->first.end(),
	      ostream_iterator<string>(cout, " "));
	cout << "->";
	copy( it2->second.first.begin(),
	      it2->second.first.end(),
	      ostream_iterator<string>(cout, " "));
	cout << endl;
      }
    }

    vector<string> v;
    v.push_back("item");

    if( optCat.isIDAttribute("Sales",
			     v,
			     "descCategory")) {
      cout << endl << "Item dependent!" << endl;
    } else
      cout << endl << "not Item Dependent!" << endl;
			 
    */
    
    string qry="MINE RULE mrArianna5 AS SELECT DISTINCT 1..n item AS BODY,1..n item AS HEAD, SUPPORT, CONFIDENCE WHERE BODY.descCategory>'a' AND HEAD.descCategory>'a' FROM Sales GROUP BY gid EXTRACTING RULES WITH SUPPORT:0.012,CONFIDENCE:0.96";
    ParsedMinerule p(qry);
    
    if(p.hasIDConstraints())
      cout << "IDC" << endl;
    else
      cout << "not IDC" << endl;

    //    cout << optCat.getCatalogue(OptimizerCatalogue::DEPFUN);
  } catch (std::exception& e) {
    cout << e.what() << endl;
  } 
}
