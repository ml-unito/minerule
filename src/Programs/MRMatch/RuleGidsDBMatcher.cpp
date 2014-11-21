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
#include "RuleGidsDBMatcher.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/mrdb/Connection.hpp"
#include "minerule/mrdb/Statement.hpp"
#include "minerule/mrdb/PreparedStatement.hpp"


namespace mrmatch {
	void RuleGidsDBMatcher::createOutputTable(const minerule::ItemType& item) const {
		mrdb::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		std::auto_ptr<mrdb::Statement> state(con->createStatement());
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
		mrdb::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		std::auto_ptr<mrdb::Statement> state(con->createStatement());

		minerule::MRLog() << "Building index on body/head..." << std::endl;
		state->execute("\
			CREATE INDEX " + indexName +
			" ON " + _outTableName + indexCols);
	}
	
	
	void RuleGidsDBMatcher::printMatches() const {
		bool outTableCreated = false;
		
		minerule::MRLogPusher _("Saving matches onto the database...");
		mrdb::Connection*  con = minerule::MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		
		minerule::MRLog() << "Inserting rules..." << std::endl;
		std::auto_ptr<mrdb::PreparedStatement> ps(con->prepareStatement("\
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