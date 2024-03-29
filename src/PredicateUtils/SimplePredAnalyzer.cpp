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
#include "minerule/PredicateUtils/SimplePredAnalyzer.hpp"


namespace minerule {


  
  const char* SimplePredAnalyzer::opRelationTable[6][6]=
     //                Yop
     //  -------------------------------
    {//  <     <=   =      >=    >    <>
      {"lbr","lrr","l!!","/!!","/!!","/rr"}, // <   |
      {"llr","lbr","ll!","/=!","/!!","/<r"}, // <=  |
      {"!!r","!rr","!b!","rr!","r!!","r!r"}, // =   | Xop
      {"!!/","!=/","!ll","rbl","rll","r>/"}, // >=  |
      {"!!/","!!/","!!l","rrl","rbl","rr/"}, // >   |
      {"ll/","l</","l!l","/>l","/ll","/b/"}  // <>  |
    };

  /*const char* SimplePredAnalyzer::opRelationTable[6][6]=
     //                Yop
     //  -------------------------------
    {//  <     <=   =      >=    >    <>
      {"rbl","rll","r!!","/!!","/!!","/ll"}, // <   |
      {"rrl","rbl","rr!","/=!","/!!","/<l"}, // <=  |
      {"!!l","!ll","!b!","ll!","l!!","l!l"}, // =   | Xop
      {"!!/","!=/","!rr","lbr","lrr","l>/"}, // >=  |
      {"!!/","!!/","!!r","llr","lbr","ll/"}, // >   |
      {"rr/","r</","r!r","/>r","/rr","/b/"}  // <>  |
      }; */


  // operators are ordered as follows: "< <= = >= > <>"
  size_t SimplePredAnalyzer::getOperatorIndex(const char* op) 
    {

    if(op[0]=='<') {
      if(op[1]=='\0')
	return 0;
      if(op[1]=='=')
	return 1;
      if(op[1]=='>')
	return 5;
      
      throw MineruleException(MR_ERROR_INTERNAL, (std::string) "Operator "+op+" not recognized");
    } else if(op[0]=='>') {
      if(op[1]=='\0')
	return 4;
      if(op[1]=='=')
	return 3;

      throw MineruleException(MR_ERROR_INTERNAL,(std::string) "Operator"+op+" not recognized");
    } else  if(op[0]=='=') {
      if(op[1]=='\0')
	return 2;
    }

    throw MineruleException(MR_ERROR_INTERNAL,(std::string) "Operator"+op+" not recognized");
  }

  size_t SimplePredAnalyzer::getValuesRelationshipIndex(const char* value1, 
							const char* value2, 
							SQLUtils::Type type) 
     {

    double order;
    if( type==SQLUtils::String) {
      order=strcmp(value1,value2);
    } else if( type==SQLUtils::Numeric ) {
      order =
	Converter(value1).toDouble()-Converter(value2).toDouble();
    } else {
      throw MineruleException(MR_ERROR_INTERNAL,
                 "SimplePredAnalyzer still does not support SQL types"
		 " which differ fromstd::string and numeric");
    }

    /*
     * It seems that the table is built using the order x>y x=y x<y...
     */
    if( order<0 )
      return 2;
    else if(order==0)
      return 1;
    else return 0;
  }

  bool
  SimplePredAnalyzer::isAttrOpValuePredicate(simple_pred* X,
					     char*& attr,
					     char*& value,
					     bool& reverseOp) {
    if(SQLUtils::isAttribute(X->val1)) {
      attr=X->val1;
      value=X->val2;
      reverseOp=false;
    } else if(SQLUtils::isAttribute(X->val2)) {
      attr=X->val2;
      value=X->val1;
      reverseOp=true;
    }

    if(attr==NULL || SQLUtils::isAttribute(value))
      return false;
    else
      return true;
  }

  char 
  SimplePredAnalyzer::getRelation(const char* Xop,
				  const char* Xvalue,
				  const char* Yop,
				  const char* Yvalue,
				  SQLUtils::Type type) 
    {

    size_t XopIndex = getOperatorIndex(Xop);
    size_t YopIndex = getOperatorIndex(Yop);
    size_t XYvalsIndex = getValuesRelationshipIndex(Xvalue,Yvalue,type);
    return opRelationTable[XopIndex][YopIndex][XYvalsIndex];
  }
  
} // namespace minerule
