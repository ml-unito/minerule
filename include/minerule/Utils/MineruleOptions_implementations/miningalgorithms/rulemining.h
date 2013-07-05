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
class RulesMiningAlgorithms : public OptionBase {
public:
	class PartitionBase : public OptionBase {
	  unsigned int rowsPerPartition;
	public:
	  virtual ~PartitionBase() {};

	  virtual std::string className() const {
	    return "partitionbase";
	  }

	  virtual void setOption(const std::string& name,const std::string& value) 
	    throw(MineruleException);
	  unsigned int getRowsPerPartition() const {
	    return rowsPerPartition;
	  }
	  void setRowsPerPartition(unsigned int num) {
	    rowsPerPartition = num;
	  }
	};

	class PartitionWithClusters : public OptionBase {
	  unsigned int rowsPerPartition;
	public:
	  virtual ~PartitionWithClusters() {};

	  virtual std::string className() const {
	    return "partitionwithclusters";
	  }

	  virtual void setOption(const std::string& name,const std::string& value) 
	    throw(MineruleException);
	  unsigned int getRowsPerPartition() const {
	    return rowsPerPartition;
	  }
	  void setRowsPerPartition(unsigned int num) {
	    rowsPerPartition = num;
	  }

	};

	class FPGrowth : public OptionBase {
	public:
	  typedef enum {
	    Original,
	    SingleReorder
	  } FPAlgoType;
	private:

	  FPAlgoType algoType;
	public:
	  virtual ~FPGrowth() {};
	  virtual std::string className() const {
	    return "fpgrowth";
	  }

	  virtual void setOption(const std::string& name,const std::string& value) 
	    throw(MineruleException);

	  FPAlgoType getAlgoType() const {
	    return algoType;
	  }
	  void setAlgoType(FPAlgoType type) {
	    algoType = type;
	  }
	};

	// --- MiningAlgorithms methods and variables

	PartitionBase partitionbase;
	PartitionWithClusters partitiongeneralized;
	FPGrowth fpgrowth;

	AlgorithmTypes preferredAlgorithm;

	PartitionBase&
	  getPartitionBase()   {
	  return partitionbase;
	}  

	const PartitionBase&
	  getPartitionBase() const  {
	  return partitionbase;
	}  

	PartitionWithClusters&
	  getPartitionWithClusters()   {
	  return partitiongeneralized;
	}  

	const PartitionWithClusters&
	  getPartitionWithClusters() const  {
	  return partitiongeneralized;
	}  

	FPGrowth& 
	  getFPGrowth()  {
	  return fpgrowth;
	}

	const FPGrowth& 
	  getFPGrowth() const {
	  return fpgrowth;
	}

	virtual void setOption(const std::string& name,const std::string& value) 
	  throw(MineruleException);

	virtual std::string className() const {
	  return "rulesmining";
	}

	virtual OptionBase& subclassForName(const std::string& subclassName) 
	  throw(MineruleException) {
	  if( subclassName=="partitionbase" ) {
	    return partitionbase;
	  } else if(subclassName=="partitionwithclusters") {
	    return partitiongeneralized;
	  } else if(subclassName=="fpgrowth") {
	    return fpgrowth;
	  } else {
	    return OptionBase::subclassForName(subclassName);
	  }
	}



	RulesMiningAlgorithms() : preferredAlgorithm(ATNone) {  }

	RulesMiningAlgorithms(const RulesMiningAlgorithms& rhs) :
	  partitionbase(rhs.partitionbase),
	  partitiongeneralized(rhs.partitiongeneralized),
	  fpgrowth(rhs.fpgrowth),
	  preferredAlgorithm(rhs.preferredAlgorithm) {};

	virtual ~RulesMiningAlgorithms() {}

	void setPreferredAlgorithm(AlgorithmTypes type) {
	  preferredAlgorithm = type;
	}

	AlgorithmTypes getPreferredAlgorithm() const {
	  return preferredAlgorithm;
	}
}; /* End of RuleMiningAlgorithms options */

