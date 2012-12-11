#ifndef _CONSTRAINTS_H
#define _CONSTRAINTS_H
#include "Utils/MineruleOptions.h"
#include "Database/MRResultSet.h"
#include "Utils/Bitstring.h"

namespace minerule { 

class BodyMap;

class AggregateConstraint {
  public:
	int attributeIndex;
	AggregateConstraint (int ai) : attributeIndex(ai) {}
	AggregateConstraint () : attributeIndex(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items) =0;
	virtual bool check (Transaction& items) =0;
	friend std::istream& operator>>(std::istream& in, AggregateConstraint& i1) {
		in >> i1.attributeIndex;
		return in;
	}
	friend std::ostream& operator<<(std::ostream& out, const AggregateConstraint& i1) {
		out << i1.attributeIndex;
		return out;
	}
};


class AggregateMonoConstraint : public AggregateConstraint {
  public:
//	AggregateMonoConstraint (int ai) : AggregateConstraints(ai) {}
//	AggregateMonoConstraint () : AggregateConstraints(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items) =0;
	virtual bool check (Transaction& items) =0;
};

class AggregateAntiMonoConstraint : public AggregateConstraint {
  public:
//	AggregateAntiMonoConstraint (int ai) : AggregateConstraints(ai) {}
//	AggregateAntiMonoConstraint () : AggregateConstraints(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items) =0;
	virtual bool check (Transaction& items) =0;
};

class SumLessThan : public AggregateAntiMonoConstraint {
	int value;
  public:
	SumLessThan (int ai, int v) : value(v) { attributeIndex = ai;}
	SumLessThan (int v) : value(v)  {}
	SumLessThan () : value(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items);
	virtual bool check (Transaction& items);
};

class SumLessEq : public AggregateAntiMonoConstraint {
	int value;
  public:
	SumLessEq (int ai, int v) : value(v) { attributeIndex = ai;}
	SumLessEq (int v) : value(v)  {}
	SumLessEq () : value(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items);
	virtual bool check (Transaction& items);
};

class SumGreaterThan : public AggregateMonoConstraint {
	int value;
  public:
	SumGreaterThan (int ai, int v) : value(v) { attributeIndex = ai;}
	SumGreaterThan (int v) : value(v)  {}
	SumGreaterThan () : value(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items);
	virtual bool check (Transaction& items);
};

class SumGreaterEq : public AggregateMonoConstraint {
	int value;
  public:
	SumGreaterEq (int ai, int v) : value(v) { attributeIndex = ai;}
	SumGreaterEq (int v) : value(v)  {}
	SumGreaterEq () : value(0) {}
	virtual bool check (BodyMap& itemMap, std::vector<ItemType>& items);
	virtual bool check (Transaction& items);
};

} // end namespace

#endif
