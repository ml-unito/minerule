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
		readElems(  rs_rules->getInt(1), *body, body_elems );
		readElems(  rs_rules->getInt(2), *head, head_elems );
		r.setBody(body);
		r.setHead(head);
		r.setSupport(rs_rules->getFloat(3));
		r.setConfidence(rs_rules->getFloat(4));
	}

} // namespace
