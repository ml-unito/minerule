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
#ifndef RULEGIDSDBMATCHER_H_45M76FV9
#define RULEGIDSDBMATCHER_H_45M76FV9

#include "RuleGidsMatcher.h"

namespace mrmatch {
	class RuleGidsDBMatcher : public RuleGidsMatcher {
	protected:
		std::string _outTableName;
		minerule::ParsedMinerule _minerule;
		
		void createOutputTable(const minerule::ItemType& item) const;
		void createOutputTableIndex(const std::string& indexName, const std::string& indexCols) const;
	public:
		RuleGidsDBMatcher(const std::string& outTableName, const minerule::ParsedMinerule& minerule) 
			: RuleGidsMatcher(), _outTableName(outTableName), _minerule(minerule) {}
		virtual ~RuleGidsDBMatcher() {}
		
		virtual void printMatches() const;
	};
}

#endif /* end of include guard: RULEGIDSDBMATCHER_H_45M76FV9 */


