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
#include "minerule/Optimizer/OptimizerCatalogue.h"
#include "minerule/Utils/MineruleOptions.h"
#include "minerule/Database/Connection.h"

namespace minerule {

	void 
		QueryResult::Iterator::readElems(int id, ItemSet& elems, odbc::PreparedStatement* ps_elems) 
	throw (MineruleException, odbc::SQLException, std::exception) {
		ps_elems->setInt(1,id);
		odbc::ResultSet* rs = ps_elems->executeQuery();

		try {
			while( rs->next() ) {
				std::string str = rs->getString(1);
				ItemType elem;
				elem.setSourceRowElement(*SourceRowElement::deserializeElementFromResultSet(rs,2));
				elems.push_back(elem);
			}
		} catch (...) {
			delete rs;
			throw;
		}
  
		delete rs;
	}


	void
		QueryResult::Iterator::init( const std::string& rulesTable,
			double support,
	double confidence ) throw( MineruleException, odbc::SQLException ) {
		odbc::Connection* odbc_conn = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		Connection connection;
		connection.setOutTableName(rulesTable);
			
		state=odbc_conn->createStatement();
		std::string query = 
			"SELECT bodyId, headId, supp, con "
				"FROM " + connection.getTableName(Connection::RulesTable) + " "
					"WHERE supp>=" + Converter(support).toString() + " AND "
						"con>="+Converter(confidence).toString();

		body_elems = odbc_conn->prepareStatement(
			"SELECT * FROM "+ connection.getTableName(Connection::BodiesTable) +
				" WHERE id=?");

		head_elems = odbc_conn->prepareStatement(
			"SELECT * FROM "+ connection.getTableName(Connection::HeadsTable) +
				" WHERE id=?");


		MRDebug() << "QueryResult::Iterator, the filter query is:" << query << std::endl;

		rs_rules = state->executeQuery(query);
	}

	bool QueryResult::Iterator::next() {
		return rs_rules->next();
	}

	void QueryResult::Iterator::getRule( Rule& r ) throw(MineruleException, odbc::SQLException, std::exception) {
		ItemSet* body = new ItemSet();
		ItemSet* head = new ItemSet();
		size_t bid, hid;

		bid = rs_rules->getInt(1);
		hid = rs_rules->getInt(2);
		
		readElems( bid, *body, body_elems );
		readElems( hid, *head, head_elems );
		
		r.setBody(body);
		r.setHead(head);
		r.setSupport(rs_rules->getFloat(3));
		r.setConfidence(rs_rules->getFloat(4));
		r.setBodyId(bid);
		r.setHeadId(hid);
	}

} // namespace
