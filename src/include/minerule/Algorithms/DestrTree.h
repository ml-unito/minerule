#ifndef __DESTRUCTIVE_INCREMENTAL_ALGO_H__
#define __DESTRUCTIVE_INCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "IncrementalAlgorithm.h"
#include "Database/ItemType.h"
#include "Algorithms/IncrAlgoClasses.h"
#include "Database/MIndex.h"

namespace minerule {

/**
 * This class implements <COMPLETE>
 */

class DestrTree : public IncrementalAlgorithm {
 protected:
  Body* root;
  size_t ngroups;
  MIndexIterator* mb1;
  MIndexIterator* mh1;
  odbc::ResultSet* rb1;
  odbc::ResultSet* rh1;
  MIndexIterator* mb1nb2;
  MIndexIterator* mh1nh2;
  odbc::ResultSet* rb1nb2;
  odbc::ResultSet* rh1nh2;
  SourceRowColumnIds bodyDes;
  SourceRowColumnIds headDes;

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

  std::string buildQry1NotQry2( const std::string& groupAttrStr,
				const std::string& attrStr,
				const std::string& constraint1,
				const std::string& constraint2) const;

  Body* getRoot(){return root;}
 public:
  DestrTree(const OptimizedMinerule& mr) :
    IncrementalAlgorithm(mr),
    root(new Body()),
    ngroups(0),
    mb1(NULL),
    mh1(NULL),
    rb1(NULL),
    rh1(NULL),
    mb1nb2(NULL),
    mh1nh2(NULL),
    rb1nb2(NULL),
    rh1nh2(NULL) {
  }

  virtual ~DestrTree() {
    delete root;
  }

  virtual void execute()
    throw(MineruleException,odbc::SQLException);
};

} // namespace

#endif
