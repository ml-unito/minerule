#ifndef __RULEFORMATTER_H__
#define __RULEFORMATTER_H__

#include <vector>
#include <iostream>
#include <set>
#include "Database/QueryResult.h"


namespace minerule {
/**
	* This is the base class for all RuleFormatter objects
 */
	class RuleFormatter {
	protected:
		std::string _fieldSeparationString;
		bool _suppressLog;

		static std::string quote(const std::string& elem);
		static std::string quoteElems(const ItemSet& elems);
	public:
		RuleFormatter() : _fieldSeparationString(""), _suppressLog(false) {
		}

		virtual ~RuleFormatter() {}

		virtual void
			printRule(const Rule&) = 0;

		void setFieldSeparationString(const std::string& s) {
			_fieldSeparationString=s;
		}
  
		void setSuppressLog(bool newVal) {  _suppressLog = newVal; }
		bool suppressLog() const { return _suppressLog; }

		virtual void
			postExec()=0;
	};


	class SimpleRuleFormatter : public RuleFormatter {
	public:
		SimpleRuleFormatter() : RuleFormatter() {};

		virtual ~SimpleRuleFormatter() {};

		virtual void
			printRule(const Rule&);

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
