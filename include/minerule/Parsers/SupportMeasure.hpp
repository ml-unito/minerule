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
