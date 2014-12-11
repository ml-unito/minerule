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
class Optimizations: public OptionBase {
public:
  typedef enum  {
	ConstructiveAlgo,
	DestructiveAlgo,
	AutochooseIncrAlgo
  } PreferredIncrementalAlgorithm;

  class Combinator: public OptionBase {
  private:
	float  timeOut;
	size_t maxDisjuncts;
	size_t maxQueries;
	size_t maxDistinctPredicates;
  public:
	Combinator() :
	  timeOut(4.0),
	  maxDisjuncts(3),
	  maxQueries(5),
	  maxDistinctPredicates(10) {}
	virtual ~Combinator() {};

	
	virtual std::string className() const {
	  return "combinator";
	}
	
	virtual void setOption(const std::string& name, const std::string& value)
	  ;

	float getTimeOutThreshold() const { return timeOut; }
	size_t getMaxDisjuncts() const {return maxDisjuncts;}
	size_t getMaxQueries() const {return maxQueries;}
	size_t getMaxDistinctPredicates() const {return maxDistinctPredicates;}
	
	void setMaxDisjuncts(size_t m) { maxDisjuncts=m; }
	void setMaxQueries(size_t m) { maxQueries=m; }
	void setMaxDistinctPredicates(size_t m) { maxDistinctPredicates=m; }
	void setTimeOutThreshold(float to) { timeOut=to; }
  };

private:
  bool tryOptimizations;
  OptimizerCatalogue* catalogue;
  PreferredIncrementalAlgorithm preferredIncrementalAlgorithm;
  bool avoidDominanceDetection;
  bool avoidEquivalenceDetection;
  bool avoidCombinationDetection;
  Combinator combinator;
public:
  Optimizations() : 
	catalogue(NULL), 
	preferredIncrementalAlgorithm(AutochooseIncrAlgo),
	avoidDominanceDetection(false),
	avoidEquivalenceDetection(false),
	avoidCombinationDetection(false) {};

  virtual ~Optimizations() {
	if( catalogue!=NULL )
	  delete catalogue;
  };

  virtual std::string className() const {
	return "optimizations";
  }

  virtual void setOption(const std::string& name,const std::string& value) 
	;

  virtual OptionBase& subclassForName(const std::string& subclassName) 
  {
	if( subclassName=="combinator" ) {
	  return combinator;
	} else {
	  return OptionBase::subclassForName(subclassName);
	}
  }

  Combinator& getCombinator() { return combinator; }
  const Combinator& getCombinator() const { return combinator; }

  bool getTryOptimizations() const {
	return tryOptimizations;
  }

  void setTryOptimizations(bool val) {
	tryOptimizations=val;
  }

  bool getAvoidEquivalenceDetection() const {
	return avoidEquivalenceDetection;
  }

  void setAvoidEquivalenceDetection(bool val) {
	avoidEquivalenceDetection=val;
  }

  bool getAvoidCombinationDetection() const {
	return avoidCombinationDetection;
  }

  void setAvoidCombinationDetection(bool val) {
	avoidCombinationDetection=val;
  }

  bool getAvoidDominanceDetection() const {
	return avoidDominanceDetection;
  }

  void setAvoidDominanceDetection(bool val) {
	avoidDominanceDetection=val;
  }
  
  PreferredIncrementalAlgorithm 
  getIncrementalAlgorithm() const {
	return preferredIncrementalAlgorithm;
  }

  void setIncrementalAlgorithm(PreferredIncrementalAlgorithm p) {
	preferredIncrementalAlgorithm=p;
  }

  OptimizerCatalogue& getCatalogue() {
	if( catalogue==NULL ) {
	  catalogue = new OptimizerCatalogue();
	}

	return *catalogue;
  }
};
