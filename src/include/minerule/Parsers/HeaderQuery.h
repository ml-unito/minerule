#ifndef __HEADERQUERY_H__
#define __HEADERQUERY_H__
#include <string>
#include "Utils/MinMaxPair.h"
#include "ParsedMinerule.h"

namespace minerule {

  /**
   *  This class is used by the parser to encapsulate the header data in order to distinguish between itemsets and rules
   */
  class HeaderQuery{
  
private:
    std::string name;
    MinMaxPair* cardB;
    MinMaxPair* cardH;
    ParsedMinerule::ListType* attrB;
    ParsedMinerule::ListType* attrH;
  
 public:     
    
    /**
     + this constructor build an object for a mine rule query
     */
    HeaderQuery(std::string& na, 
		MinMaxPair* cB, 
		MinMaxPair* cH, 
		ParsedMinerule::ListType* aB,
		ParsedMinerule::ListType* aH) : 
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
                ParsedMinerule::ListType* aB) :
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
    
    ParsedMinerule::ListType* getAttrBody(){
      return attrB;
    }
    
    ParsedMinerule::ListType* getAttrHead(){
      return attrH;
    }
    
  };
}//end namespace
#endif
