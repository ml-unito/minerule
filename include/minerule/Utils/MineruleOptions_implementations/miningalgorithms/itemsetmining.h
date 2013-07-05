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
