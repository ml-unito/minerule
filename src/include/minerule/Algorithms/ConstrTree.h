#ifndef __CONSTRUCTIVE_INCREMENTAL_ALGO_H__
#define __CONSTRUCTIVE_INCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "Algorithms/IncrementalAlgorithm.h"
#include "Database/ItemType.h"
#include "Algorithms/IncrAlgoClasses.h"
#include "Database/MIndex.h"

namespace minerule {

/**
 * This class implements <COMPLETE>
 */

class ConstrTree : public IncrementalAlgorithm {
 protected:
  Body* root;
  size_t ngroups;
  MIndexIterator* mb2;
  MIndexIterator* mh2;
  odbc::ResultSet* rb2;
  odbc::ResultSet* rh2;
  SourceRowColumnIds bodyDes;
  SourceRowColumnIds headDes;
  odbc::Statement* stateb2;
  odbc::Statement* stateh2;

  void adjustSuppRSet();
  void adjustSuppMIndex();
  void insertRulesInStructure();
  void adjustSupp();
  void prepareData();

  size_t buildAttrStr(const ParsedMinerule::AttrVector& attr,
		      size_t startIndex,
		      std::string& attrStr, 
		      std::vector<int>& des) const;

  std::string buildQry( const std::string& groupAttrStr,
			const std::string& attrStr,
			const std::string& constraints) const;

  Body* getRoot(){return root;}
 public:
  ConstrTree(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr), root(new Body()), ngroups(0), mb2(NULL), mh2(NULL), rb2(NULL), rh2(NULL) { }

  virtual ~ConstrTree() {
    // Trashing the trashable
    if(rh2!=NULL) delete rh2;
    if(stateh2!=NULL) delete stateh2;
    if(rb2!=NULL) delete rb2;
    if(stateb2!=NULL) delete stateb2;
    	
    rb2=rh2=NULL;
    stateh2=stateb2=NULL;
    
	delete root;
  }

  virtual void execute() throw(MineruleException,odbc::SQLException);
};

} // namespace

#endif
