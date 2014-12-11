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
#ifndef __INCREMENTAL_ALGORITHM_H__
#define __INCREMENTAL_ALGORITHM_H__

#include "minerule/Optimizer/OptimizedMinerule.hpp"

namespace minerule {

class IncrementalAlgorithm {
 protected:  
  const OptimizedMinerule* minerule;
 public:
  IncrementalAlgorithm(const OptimizedMinerule& mr) :
    minerule(&mr) {}

  virtual ~IncrementalAlgorithm() {}

  virtual void execute() =0;

  static IncrementalAlgorithm* 
  newIncrementalAlgorithm(const OptimizedMinerule& mr);
};


} //namespace

#endif
