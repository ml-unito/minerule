#include <iostream>
#include <string>

using namespace std;

class SalesRow {
  public:
	int gid;
	int item;
	int price;
	string category;
	double discount;
  	friend istream& operator>> (istream& in, SalesRow& row) {
		in >> row.gid >> row.item >> row.price >> row.category >> row.discount;
		return in;
	}
  	friend ostream& operator<< (ostream& out, SalesRow& row) {
		out << row.gid << ' ' << row.item << ' ' << row.price << ' ' << row.category << ' ' << row.discount << endl;
		return out;
	}
};

class IndexElem {
  public:
	int rowPos;
	int gid;
	IndexElem() {}
	IndexElem(char * v) { rowPos = *(int*)v; gid = *(int*)(&v[sizeof(int)]); }
	friend bool operator<(const IndexElem& ie1, const IndexElem& ie2) {
		return ie1.gid < ie2.gid || (ie1.gid == ie2.gid && ie1.rowPos<ie2.rowPos);
	}
};

class Comparison {
  protected:
	int intVal;
	int intVal2;
	string strVal;
	string strVal2;
  public:
	Comparison(char * val) { if (isdigit(val[0])) intVal = atoi(val);
				 else strVal = val; }
	virtual bool compare(int v1) =0;
	virtual bool compare(string v1) =0;
};

class EqualTo : public Comparison {
  public:
	EqualTo(char * val) : Comparison(val) {}
	virtual bool compare(int v1) { return v1 == intVal; }
	virtual bool compare(string v1) { return v1 == strVal; };
};

class LessThan : public Comparison {
  public:
	LessThan(char * val) : Comparison(val) {}
	virtual bool compare(int v1) { return v1 < intVal; }
	virtual bool compare(string v1) { return v1 < strVal; };
};


class LessOrEqual : public Comparison {
  public:
	LessOrEqual(char * val) : Comparison(val) {}
	virtual bool compare(int v1) { return v1 <= intVal; }
	virtual bool compare(string v1) { return v1 <= strVal; };
};


class GreaterThan : public Comparison {
  public:
	GreaterThan(char * val) : Comparison(val) {}
	virtual bool compare(int v1) { return v1 > intVal; }
	virtual bool compare(string v1) { return v1 > strVal; };
};


class GreaterOrEqual : public Comparison {
  public:
	GreaterOrEqual(char * val) : Comparison(val) {}
	virtual bool compare(int v1) { return v1 >= intVal; }
	virtual bool compare(string v1) { return v1 >= strVal; };
};


class InBetween : public Comparison {
  public:
	InBetween(char * val1, char * val2) : Comparison(val1) {
		if (isdigit(val2[0])) intVal2 = atoi(val2);
		else strVal2 = val2;
	}
	virtual bool compare(int v1) { return v1 >= intVal && v1 <= intVal2; }
	virtual bool compare(string v1) { return v1 >= strVal && v1 <= strVal2; };
};


