#ifndef __RULEFORMATTER_H__
#define __RULEFORMATTER_H__

#include <vector>
#include <iostream>
#include <set>
#include "Result/QueryResult.h"


namespace minerule {
/**
	* This is the base class for all RuleFormatter objects
 */
	class RuleFormatter {
	public:
		class FieldWidths {
		public:
			size_t body;  // width of the body column
			size_t head;  // width of the head column
			size_t supp;
			size_t conf;
			
			FieldWidths(size_t b,size_t h,size_t s,size_t c) : body(b), head(h), supp(s), conf(c) {}
		};
		
	protected:
		bool _suppressLog;
		
		std::string _bhSep;   // separates body from head 
		FieldWidths _fieldWidths;
				

		static std::string quote(const std::string& elem);
		static std::string quoteElems(const ItemSet& elems);
	public:
		RuleFormatter() : _suppressLog(false), _bhSep("=>"), _fieldWidths(30,30,9,9) {
		}

		virtual ~RuleFormatter() {}

		virtual std::string formatRule(const Rule&) = 0;
		virtual void printRule(const Rule&) = 0;
  
		void setSuppressLog(bool newVal) {  _suppressLog = newVal; }
		bool suppressLog() const { return _suppressLog; }
		
		void setFieldWidths( const FieldWidths& f ) { _fieldWidths = f; }
		FieldWidths& fieldWidths() { return _fieldWidths; }
		const FieldWidths& fieldWidths() const { return _fieldWidths; }

		virtual void
			postExec()=0;
	};


	class SimpleRuleFormatter : public RuleFormatter {
	public:
		SimpleRuleFormatter() : RuleFormatter() {};

		virtual ~SimpleRuleFormatter() {};

		virtual std::string formatRule(const Rule&);
		virtual void printRule(const Rule&);

		virtual void
			postExec() {};
	};


template <class Sorter>
	class SortedRuleFormatter : public SimpleRuleFormatter {
		typedef std::set<Rule,Sorter> SortedContainer;
		SortedContainer sortedRules;
	public:
		SortedRuleFormatter() : SimpleRuleFormatter() {};
		virtual ~SortedRuleFormatter() {};

		virtual void printRule(const Rule&);

		virtual void postExec();
	};

template <class Sorter> 
	void SortedRuleFormatter<Sorter>::printRule(const Rule& r) {
		sortedRules.insert(r);
	}

template <class Sorter> 
	void SortedRuleFormatter<Sorter>::postExec() {
		typename SortedContainer::const_iterator it;
		for(it=sortedRules.begin(); it!=sortedRules.end(); it++) {
			SimpleRuleFormatter::printRule(*it);
		}
	}

} // namespace

#endif
