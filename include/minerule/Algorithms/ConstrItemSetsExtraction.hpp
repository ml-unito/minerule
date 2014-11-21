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
#ifndef __CONSTRITEMSETSEXTRACTION_H
#define __CONSTRITEMSETSEXTRACTION_H

#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Database/Connection.hpp"
#include "minerule/Algorithms/Bodymap.hpp"

namespace minerule {

  class ConstrItemSetsExtraction : public MiningAlgorithm {
  private:
    AlgorithmsOptions options;
    Connection connection;
	SourceTable* sourceTable;
	SourceTable::Iterator bodyIterator;


    void prepareData();

  public:
    ConstrItemSetsExtraction(const OptimizedMinerule& mr) :
      MiningAlgorithm(mr) {}

    virtual ~ConstrItemSetsExtraction() {}

    virtual void mineRules();

    virtual bool canHandleMinerule() const {
      return
        !minerule.getParsedMinerule().hasCrossConditions() &&
        !minerule.getParsedMinerule().requiresClusters() &&
        !minerule.getParsedMinerule().hasDisjuctionsInMC();
    }

  };
}



#endif
