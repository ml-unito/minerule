class ItemsetsMiningAlgorithms : public OptionBase {
public:

	ItemsetsMiningAlgorithms() : preferredAlgorithm(ATNone) {  }

	ItemsetsMiningAlgorithms(const ItemsetsMiningAlgorithms& rhs) :
	  preferredAlgorithm(rhs.preferredAlgorithm) {};

	virtual ~ItemsetsMiningAlgorithms() {}

	void setPreferredAlgorithm(AlgorithmTypes type) {
	  preferredAlgorithm = type;
	}

	AlgorithmTypes getPreferredAlgorithm() const {
	  return preferredAlgorithm;
	}

	virtual std::string className() const {
	  return "itemsetsmining";
	}


	virtual OptionBase& subclassForName(const std::string& subclassName) 
	  throw(MineruleException) {
	    return OptionBase::subclassForName(subclassName);
	}

	virtual void setOption(const std::string& name,const std::string& value) 
	  throw(MineruleException);


private:
	AlgorithmTypes preferredAlgorithm;
}; // class ItemSetMiningAlgorithms
