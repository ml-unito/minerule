class MiningAlgorithms : public OptionBase {
public:
  /* ========= RULE MINING ALGORITHMS ======== */
  	#include "Utils/MineruleOptions_implementations/miningalgorithms/rulemining.h"

  /* ========= ITEMSET MINING ALGORITHMS ======== */
	#include "Utils/MineruleOptions_implementations/miningalgorithms/itemsetmining.h"
  /* ========= SEQUENCE MINING ALGORITHMS ======= */
	#include "Utils/MineruleOptions_implementations/miningalgorithms/sequencemining.h"

private:
  RulesMiningAlgorithms rulesMiningAlgorithms;
  ItemsetsMiningAlgorithms itemsetsMiningAlgorithms;
  SequencesMiningAlgorithms sequencesMiningAlgorithms; 

public:

  virtual ~MiningAlgorithms() { };

  virtual std::string className() const { 
	return "MiningAlgorithm"; 
  }

  virtual void setOption(const std::string& name,const std::string& value) throw(MineruleException) {
	throw MineruleException( MR_ERROR_OPTION_PARSING, "MiningAlgorithms option class does not support option:"+value );
  }

  virtual OptionBase& subclassForName(const std::string& subclassName) throw(MineruleException) {
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
