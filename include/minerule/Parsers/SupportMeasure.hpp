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
#ifndef _SUPPORTMEASURE__H_
#define _SUPPORTMEASURE__H_

#include "EvaluationMeasure.hpp"


namespace minerule{
/*
 * this class is used to encapsule the support
 **/
  class SupportMeasure:  public EvaluationMeasure {

 private:
  EvaluationMeasure::RelOperator relOp;
  double threshold;

 public:

  SupportMeasure(EvaluationMeasure::RelOperator rel, double th): relOp(rel), threshold(th) {};

  //virtual bool eval(ItemSet, ItemSet, double, double, double ) { assert(false); }

  /*
   * this method return if a frequency of an itemset satisfy the support or not satisfy this measure
   **/
  bool evalSupport(double freq);


  /*
   * this method return the type of this comparison
   * check the relational opertor to establish if a measure is monotone or antimonotone
   **/
  EvaluationMeasure::MeasureType getMeasureType();
  

  /*
   * This method return the threshold of this support
   **/
  double getThreshold(){
    return threshold;
  }

};


}//end namespace
//esempio utilizzo

//if( support.getMeasureType() == EvaluationMeasure::MONOTONE )

#endif
