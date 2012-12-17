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
