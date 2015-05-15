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
#include "minerule/Algorithms/AlgorithmsOptions.hpp"

using namespace minerule;

void AlgorithmsOptions::setSupport(double sup) { support = sup; }

void AlgorithmsOptions::setConfidence(double conf) { confidence = conf; }

void AlgorithmsOptions::setSourceRowDescription(
    const SourceRowColumnIds &srdes) {
  sourceRowDescription = srdes;
}

double AlgorithmsOptions::getSupport() const { return support; }

double AlgorithmsOptions::getConfidence() const { return confidence; }

/*
unsigned int
AlgorithmsOptions::getRowsPerPartition() const {
  return rowsPerPartition;
  }*/


const SourceRowColumnIds &AlgorithmsOptions::getSourceRowDescription() const {
  return sourceRowDescription;
}
