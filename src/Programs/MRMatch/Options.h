#ifndef OPTIONS_H_U6THS9P
#define OPTIONS_H_U6THS9P

#include <string>
#include <vector>

#include "mrmatch.h"

namespace mrmatch {
	class Options {
	public:
		Options() : _queryNumber(0), _matchKind(RuleGids) {};
		virtual ~Options () {};
		
		void setQueryName(std::string name) { _queryName   = name; 		}
		void setQueryNumber(size_t number)  { _queryNumber = number;  }
		void setTableName(const std::string&);
		const std::string& tableName() const;		
		std::string queryName() const;
		
		MatchKind matchKind() const { return _matchKind; }
		void setMatchKind(MatchKind kind) { _matchKind = kind; }
		
		bool initMineruleOptions() const;
		
		void printUsage(int argc, char* const argv[]);
		void parse(int argc, char* const argv[]);
	private:
		std::string _mrOptionsFileName;
		std::vector<std::string> _mrOptionsInline;		
		
		std::string _queryName;
		std::string _tableName;
		size_t _queryNumber;
		
		MatchKind _matchKind;
		
		static MatchKind stringToMatchKind(const std::string&);
	};

}

#endif /* end of include guard: OPTIONS_H_U6THS9P */
