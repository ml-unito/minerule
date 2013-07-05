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
