//   Minerule - a sql-like language for datamining
//   Copyright (C) 2013 Roberto Esposito (esposito@di.unito.it)
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
#include "minerule/Algorithms/MiningAlgorithmBase.hpp"
#include "minerule/Algorithms/BFSWithGidsNoCross.hpp"
#include "minerule/Algorithms/BFSWithGidsAndCross.hpp"
#include "minerule/Algorithms/Care.hpp"
#include "minerule/Algorithms/ConstrItemSetsExtraction.hpp"

namespace minerule {

MiningAlgorithmBase*
MiningAlgorithmBase::algorithmForType(AlgorithmTypes t, const OptimizedMinerule& mr) throw(MineruleException) {
  switch(t) {
  case ATNone:
    return new MiningAlgorithm(mr);
  case ATBFSWithGidsNoCross:
    return new BFSWithGidsNoCross(mr);
  case ATBFSWithGidsAndCross:
    return new BFSWithGidsAndCross(mr);
  case ATCare:
    return new CARE(mr);
  case ATConstrainedItemsets:
    return new ConstrItemSetsExtraction(mr);
  default: throw MineruleException( MR_ERROR_INTERNAL, "Requested unknown mining algorithm type");
  }
}

}