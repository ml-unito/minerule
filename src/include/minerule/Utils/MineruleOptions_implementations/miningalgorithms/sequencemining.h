class SequencesMiningAlgorithms : public OptionBase {
public:

	SequencesMiningAlgorithms() : preferredAlgorithm(ATNone) {  }

	SequencesMiningAlgorithms(const SequencesMiningAlgorithms& rhs) :
	  preferredAlgorithm(rhs.preferredAlgorithm) {}

	virtual ~SequencesMiningAlgorithms() {}

	void setPreferredAlgorithm(AlgorithmTypes type) {
	  preferredAlgorithm = type;
	}

	AlgorithmTypes getPreferredAlgorithm() const {
	  return preferredAlgorithm;
	}

	virtual std::string className() const {
	  return "sequencesmining";
	}


	virtual OptionBase& subclassForName(const std::string& subclassName) 
	  throw(MineruleException) {
	    return OptionBase::subclassForName(subclassName);
	}

	virtual void setOption(const std::string& name,const std::string& value) 
	  throw(MineruleException);


private:
	AlgorithmTypes preferredAlgorithm;
}; // class SequenceMiningAlgorithms
