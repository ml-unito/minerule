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


