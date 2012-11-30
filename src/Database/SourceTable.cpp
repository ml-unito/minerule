#include "Database/SourceTable.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include <odbc++/resultset.h>

namespace minerule {
	
	SourceTable::~SourceTable() {
		for(std::vector<odbc::ResultSet*>::const_iterator it = _managedResults.begin(); it!=_managedResults.end(); ++it) {
			delete *it;
		}
	}	

	bool SourceTable::Iterator::next() { 
		assert( _sourceRow != NULL && _resultSet != NULL );
		if( _resultSet->next() ) {
			_sourceRow->init(_resultSet, _columnIds);
			return true;
		} else {
			return false;
		}		
	}

	bool SourceTable::Iterator::isAfterLast() const {
		assert( _resultSet!=NULL );
		return _resultSet->isAfterLast();
	}

	SourceTable::Iterator SourceTable::newIterator(SourceTable::IteratorKind kind)  {
		switch(kind) {
			case BodyIterator:
				{
					_managedResults.push_back(_bodyStatement->executeQuery());
					SourceRowColumnIds bodyCols = _columnIds;
					bodyCols.headElems.clear();

					return Iterator(_managedResults.back(), bodyCols);
				}
			case HeadIterator:
				{
					_managedResults.push_back(_headStatement->executeQuery());
					SourceRowColumnIds headCols = _columnIds;
					headCols.bodyElems.clear();
					
					return Iterator(_managedResults.back(), headCols);
				}
			case FullIterator:
				{
					_managedResults.push_back(_fullStatement->executeQuery());
					return Iterator(_managedResults.back(), _columnIds);
				}
		}
	
		throw MineruleException(MR_ERROR_INTERNAL, "Uknown IteratorKind. This is a bug, please report it!");
	}
	
	size_t SourceTable::getTotGroups() {
		return _pdu.evaluateTotGroups();
	}

	void SourceTable::initBodyHeadResultSets() {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		const OptimizedMinerule& minerule = _algorithm.optimizedMinerule();
	
		std::string bodyCondition, headCondition;
	
		list_AND_node* miningCondition = (minerule.getParsedMinerule().mc!=NULL ? minerule.getParsedMinerule().mc->l_and : NULL);
		HeadBodyPredicatesSeparator::separate(miningCondition, bodyCondition, headCondition);
	
		std::string bodyQuery = _pdu.buildBodyTableQuery(_columnIds, bodyCondition);
		std::string headQuery = _pdu.buildHeadTableQuery(_columnIds, headCondition);
		
		_bodyStatement = connection->prepareStatement( bodyQuery );		
		_headStatement = connection->prepareStatement( headQuery );	
		
		MRLog("Body Query:"+bodyQuery);
		MRLog("Head Query:"+headQuery);		
	}

	void SourceTable::initFullResultSet() {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();

		std::string query = _pdu.buildExtendedSourceTableQuery(_columnIds);
		
		MRLog("Source Table Query:"+query);
		_fullStatement = connection->prepareStatement( query );
	}


	void SourceTable::init() {
		if( _algorithm.sourceTableRequirements().crossProduct() )
			initFullResultSet();
		else {
			initBodyHeadResultSets();
		}
	}

} // namespace