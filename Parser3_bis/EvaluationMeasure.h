#ifndef __SUPPORTMEASURE_H__
#define __SUPPORTMEASURE_H__


namespace minerule {

class EvaluationMeasure {
  // private:
  //  const ItemDefinitionTable* table;


 public:
  typedef enum {
    MONOTONE,
    ANTIMONOTONE,
    OTHER
  } MeasureType;

  typedef enum {
    LessEqual,
    Less,
    Greater,
    GreaterEqual
  } RelOperator;

   // void setItemDefinitionTable( const ItemDefinitionTable& tab ) { table=&tab; } ;

  virtual MeasureType getMeasureType()=0;
  //virtual bool eval(ItemSet, ItemSet, double, double, double)=0;
};


//esempio utilizzo

//if( support.getMeasureType() == EvaluationMeasure::MONOTONE )
     
}//end namespace

#endif
