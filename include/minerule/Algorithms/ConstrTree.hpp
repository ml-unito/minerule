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
#ifndef __CONSTRUCTIVE_INCREMENTAL_ALGO_H__
#define __CONSTRUCTIVE_INCREMENTAL_ALGO_H__

#include<vector>
#include<set>

#include "minerule/Algorithms/IncrementalAlgorithm.hpp"
#include "minerule/Database/ItemType.hpp"
#include "minerule/Algorithms/IncrAlgoClasses.hpp"

namespace minerule {

/**
 * This class implements <COMPLETE>
 */

class ConstrTree : public IncrementalAlgorithm {
 protected:
  Body* root;
  size_t ngroups;
  mrdb::ResultSet* rb2;
  mrdb::ResultSet* rh2;
  SourceRowColumnIds bodyDes;
  SourceRowColumnIds headDes;
  mrdb::Statement* stateb2;
  mrdb::Statement* stateh2;

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
  ConstrTree(const OptimizedMinerule& mr) : IncrementalAlgorithm(mr), root(new Body()), ngroups(0) /*,mb2(NULL), mh2(NULL)*/, rb2(NULL), rh2(NULL) { }

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

  virtual void execute() throw(MineruleException,mrdb::SQLException);
};

} // namespace

#endif
