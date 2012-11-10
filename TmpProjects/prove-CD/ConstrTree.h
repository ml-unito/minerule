#ifndef __CONSTRUCTIVE_INCREMENTAL_ALGO_H__
#define __CONSTRUCTIVE_INCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "IncrementalAlgorithm.h"
#include "Database/ItemType.h"
#include "IncrAlgoClasses.h"

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
  HeadBodySourceRowDescription bodyDes;
  HeadBodySourceRowDescription headDes;

  void adjustSuppRSet();
  void adjustSuppMIndex();
  void insertRulesInStructure();
  void adjustSupp();

  size_t buildAttrStr(const ParsedMinerule::AttrVector& attr,
		      size_t startIndex,
		      std::string& attrStr, 
		      std::vector<int>& des) const;

  std::string buildQry( const std::string& groupAttrStr,
			const std::string& attrStr,
			const std::string& constraints) const;

  Body* getRoot(){return root;}
 public:
  ConstrTree(const OptimizedMinerule& mr) :
    IncrementalAlgorithm(mr),
    root(new Body()), 
    ngroups(0), 
    mb2(NULL), 
    mh2(NULL), 
    rb2(NULL), 
    rh2(NULL) {
  }

  virtual ~ConstrTree() {
    delete root;
  }

  virtual void execute() 
    throw(MineruleException,odbc::SQLException);
};

} // namespace

#endif
