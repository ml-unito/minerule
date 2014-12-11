//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Arianna Gallo (gallo.arianna@gmail.com)
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef __DESTRUCTIVE_INCREMENTAL_ALGO_H__
#define __DESTRUCTIVE_INCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "IncrementalAlgorithm.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Algorithms/IncrAlgoClasses.hpp"

namespace minerule {

/**
 * This class implements <COMPLETE>
 */

class DestrTree : public IncrementalAlgorithm {
 protected:
  Body* root;
  size_t ngroups;
  mrdb::ResultSet* rb1;
  mrdb::ResultSet* rh1;
  mrdb::ResultSet* rb1nb2;
  mrdb::ResultSet* rh1nh2;
  SourceRowColumnIds bodyDes;
  SourceRowColumnIds headDes;
  
  mrdb::Statement* stateb1, *stateb1nb2;
  mrdb::Statement* stateh1, *stateh1nh2;
  

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

  std::string buildQry1NotQry2( const std::string& groupAttrStr,
				const std::string& attrStr,
				const std::string& constraint1,
				const std::string& constraint2) const;

  Body* getRoot(){return root;}
 public:
  DestrTree(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr), root(new Body()), ngroups(0), rb1(NULL), rh1(NULL), rb1nb2(NULL), rh1nh2(NULL) { }

  virtual ~DestrTree() {
  // Trashing the trashable
	if(rh1!=NULL) delete rh1;
	if(stateh1!=NULL) delete stateh1;
	if(rb1!=NULL) delete rb1;
	if(stateb1!=NULL) delete stateb1;
	rb1=rh1=NULL;
	stateh1=stateb1=NULL;
	  
    delete root;
  }

  virtual void execute() ;
};

} // namespace

#endif
