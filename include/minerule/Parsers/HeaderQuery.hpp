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
#ifndef __HEADERQUERY_H__
#define __HEADERQUERY_H__
#include <string>
#include "minerule/Utils/MinMaxPair.hpp"
#include "ParsedMinerule.hpp"

namespace minerule {

  /**
   *  This class is used by the parser to encapsulate the header data in order to distinguish between itemsets and rules
   */
  class HeaderQuery{
  
private:
    std::string name;
    MinMaxPair* cardB;
    MinMaxPair* cardH;
    ParsedMinerule::AttrVector* attrB;
    ParsedMinerule::AttrVector* attrH;
  
 public:     
    
    /**
     + this constructor build an object for a mine rule query
     */
    HeaderQuery(std::string& na, 
		MinMaxPair* cB, 
		MinMaxPair* cH, 
		ParsedMinerule::AttrVector* aB,
		ParsedMinerule::AttrVector* aH) : 
                    name(na), 
                    cardB(cB), 
                    cardH(cH), 
                    attrB(aB), 
                    attrH (aH) {};    
    
    /**
     + this constructor build an object for a mine itemset query
     */
    HeaderQuery(std::string& na,
                MinMaxPair* cB,
                ParsedMinerule::AttrVector* aB) :
      name(na),
      cardB(cB),
      cardH(NULL),
      attrB(aB),
      attrH (NULL) {};


    /**
     + the following methods are used for take the value of this object
     */
    
    std::string getName(){
      return name;
    } 

    MinMaxPair* getCardBody(){
      return cardB;
    }  

    MinMaxPair* getCardHead(){
      return cardH;
    }
    
    ParsedMinerule::AttrVector* getAttrBody(){
      return attrB;
    }
    
    ParsedMinerule::AttrVector* getAttrHead(){
      return attrH;
    }
    
  };
}//end namespace
#endif
