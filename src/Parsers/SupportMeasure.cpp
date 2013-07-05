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
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Parsers/SupportMeasure.h"
#include "minerule/Parsers/EvaluationMeasure.h"



namespace minerule {
/*
 * this method return if a frequency of an itemset satisfy the support or not satisfy this measure
 **/
  bool SupportMeasure::evalSupport(double freq) {
    switch (relOp){
    case EvaluationMeasure::LessEqual:
      return freq <= threshold;
	break;
    case EvaluationMeasure::Less:
      return freq < threshold;
	break;
    case EvaluationMeasure::Greater:
      return freq > threshold;
	break;
    case EvaluationMeasure::GreaterEqual:
      return freq >= threshold;
	break;
    default: assert(false);
    }
  }

  EvaluationMeasure::MeasureType SupportMeasure::getMeasureType(){
    if(relOp == EvaluationMeasure::LessEqual || relOp == EvaluationMeasure::Less)
      return EvaluationMeasure::MONOTONE;
    else
      if(relOp == EvaluationMeasure::GreaterEqual || relOp == EvaluationMeasure::Greater)
        return EvaluationMeasure::ANTIMONOTONE;
      else assert(false);
  }





}//end namespace
