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
#ifndef OPTIONS_H_U6THS9P
#define OPTIONS_H_U6THS9P

#include <string>
#include <vector>

#include "mrmatch.h"

namespace mrmatch {
	class Options {
	public:
		Options() : _queryNumber(0), _matchKind(RuleGids), _matchOutput(OutOnConsole) {};
		virtual ~Options () {};
		
		void setQueryName(std::string name) { _queryName   = name; 		}
		void setQueryNumber(size_t number)  { _queryNumber = number;  }
		void setTableName(const std::string&);
		const std::string& tableName() const;		
		std::string queryName() const;
		
		MatcherSpecs matcherSpecs() const { return _matchKind | _matchOutput; }

		void setMatchKind(MatchKind kind) { _matchKind = kind; }
		void setMatchOutput(MatchOutput out) { _matchOutput = out; }
		void setMatchOutputTableName(const std::string& name) { _matchOutputTableName = name; }
		const std::string& getMatchOutputTableName() const { return _matchOutputTableName; }
		
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
		MatchOutput _matchOutput;
		std::string _matchOutputTableName;
		
		static MatchKind stringToMatchKind(const std::string&);
	};

}

#endif /* end of include guard: OPTIONS_H_U6THS9P */
