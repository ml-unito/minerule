//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Dino Ienco (dino.ienco@teledetection.fr)
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
#ifndef __CCSMINER_H__
#define __CCSMINER_H__

#include "minerule/Algorithms/CCSMSequence.hpp"
#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Utils/AlgorithmTypes.hpp"

namespace minerule {

  class CCSMiner : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    Connection connection;
    SourceRowColumnIds rowDes;
    mrdb::PreparedStatement* statement;

    void prepareData();
    
  public:
    CCSMiner(const OptimizedMinerule& mr, const AlgorithmsOptions& opts) : 
      MiningAlgorithm(mr),  statement(NULL) {
	options = opts;
        connection.useMRDBConnection(opts.getMRDBConnection());
}

    virtual ~CCSMiner() {}

    virtual void mineRules();

    virtual bool canHandleMinerule() const {
      return minerule.getParsedMinerule().miningTask == MTMineSequences;
    }

		virtual SourceTableRequirements sourceTableRequirements() const {
			return SourceTableRequirements(SourceTableRequirements::SortedGids);
		};

    bool find(std::vector<CCSMSequence*>*vec, CCSMSequence* elem); 

    void combina(std::vector<CCSMSequence::ResultItems>&, std::vector<CCSMSequence*>* k2, size_t k, int min_g, int max_g, double threshold, int check);

  };
}



#endif
