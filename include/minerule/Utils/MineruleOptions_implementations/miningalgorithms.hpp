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
class MiningAlgorithms : public OptionBase {
public:
  /* ========= RULE MINING ALGORITHMS ======== */
  	#include "minerule/Utils/MineruleOptions_implementations/miningalgorithms/rulemining.hpp"
  /* ========= ITEMSET MINING ALGORITHMS ======== */
	#include "minerule/Utils/MineruleOptions_implementations/miningalgorithms/itemsetmining.hpp"
  /* ========= SEQUENCE MINING ALGORITHMS ======= */
	#include "minerule/Utils/MineruleOptions_implementations/miningalgorithms/sequencemining.hpp"

private:
  RulesMiningAlgorithms rulesMiningAlgorithms;
  ItemsetsMiningAlgorithms itemsetsMiningAlgorithms;
  SequencesMiningAlgorithms sequencesMiningAlgorithms; 

public:

  virtual ~MiningAlgorithms() { };

  virtual std::string className() const { 
	return "MiningAlgorithm"; 
  }

  virtual void setOption(const std::string& name,const std::string& value) {
	throw MineruleException( MR_ERROR_OPTION_PARSING, "MiningAlgorithms option class does not support option:"+value );
  }

  virtual OptionBase& subclassForName(const std::string& subclassName) {
	if( subclassName=="rulesmining" ) {
	  return rulesMiningAlgorithms;
	}  else if(subclassName=="itemsetsmining") {
	  return itemsetsMiningAlgorithms;
	}  else if(subclassName=="sequencesmining") {
	  return sequencesMiningAlgorithms;
	  } else {
	  return OptionBase::subclassForName(subclassName);
	}
  }


  RulesMiningAlgorithms& getRulesMiningAlgorithms() { return rulesMiningAlgorithms; }
  const RulesMiningAlgorithms& getRulesMiningAlgorithms() const { return rulesMiningAlgorithms; }
  const ItemsetsMiningAlgorithms& getItemsetsMiningAlgorithms() const { return itemsetsMiningAlgorithms; }
  const   SequencesMiningAlgorithms& getSequencesMiningAlgorithms() const { return sequencesMiningAlgorithms; } 

};
