#include "RuleGidsDBMatcher.h"
#include "minerule/Utils/MineruleOptions.h"
#include <odbc++/connection.h>
#include <odbc++/statement.h>
#include <odbc++/preparedstatement.h>


namespace mrmatch {
	void RuleGidsDBMatcher::createOutputTable(const minerule::ItemType& item) const {
		odbc::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		std::auto_ptr<odbc::Statement> state(con->createStatement());
		minerule::SourceRowMetaInfo metaInfo(con, _minerule);
		
		minerule::MRLog() << "Creating dest table: " << _outTableName << std::endl;
		state->execute("\
			CREATE TABLE " + _outTableName + " \
				(\
				  bodyid integer,\
				  headid integer," + 
				  metaInfo.getGroup().getSQLDataDefinition() + ");");
		
		std::string colNamesWithCommas = metaInfo.getGroup().getSQLColumnNames();
		std::string colNamesWithUnderscores = minerule::StringUtils::join( minerule::StringUtils::split(colNamesWithCommas, ","), "_" );
		
		createOutputTableIndex(_outTableName + "_bodyhead_index", "(bodyid,headid)");
		createOutputTableIndex(_outTableName + "_" +colNamesWithUnderscores + "_index", "("+ colNamesWithCommas +")");		
	}
	
	void RuleGidsDBMatcher::createOutputTableIndex(const std::string& indexName, const std::string& indexCols) const {
		odbc::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		std::auto_ptr<odbc::Statement> state(con->createStatement());

		minerule::MRLog() << "Building index on body/head..." << std::endl;
		state->execute("\
			CREATE INDEX " + indexName +
			" ON " + _outTableName + indexCols);
	}
	
	
	void RuleGidsDBMatcher::printMatches() const {
		bool outTableCreated = false;
		
		minerule::MRLogPusher _("Saving matches onto the database...");
		odbc::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		
		minerule::MRLog() << "Inserting rules..." << std::endl;
		std::auto_ptr<odbc::PreparedStatement> ps(con->prepareStatement("\
			INSERT INTO " + _outTableName + " VALUES (?,?,?);"));
			
		for(RuleGidsVector::const_iterator it=_ruleGidsVector.begin(); it!=_ruleGidsVector.end(); ++it) {						
			Gids::const_iterator gids_it;
			for( gids_it = it->second.begin(); gids_it!=it->second.end(); ++gids_it ) {
				if(!outTableCreated) {
					createOutputTable(*gids_it);
					outTableCreated=true;
				}
				
				ps->setInt(1,it->first.getBodyId());
				ps->setInt(2,it->first.getHeadId());
				gids_it->setPreparedStatementParameters(ps.get(),3);
				ps->execute();
			}
		}
									
		minerule::MRLog() << "All done!" << std::endl;
	}
}