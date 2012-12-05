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
		
		std::string queryName() const;
		
		MatchKind matchKind() const { return _matchKind; }
		
		bool initMineruleOptions() const;
	private:
		std::string _mrOptionsFileName;
		std::vector<std::string> _mrOptionsInline;		
		
		std::string _queryName;
		size_t _queryNumber;
		
		MatchKind _matchKind;
	};

}

#endif /* end of include guard: OPTIONS_H_U6THS9P */
