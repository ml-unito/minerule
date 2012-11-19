#include "Database/SourceTable.h"
#include "PredicateUtils/HeadBodyPredicatesSeparator.h"
#include <odbc++/resultset.h>

namespace minerule {

	bool SourceTable::Iterator::next() { 
		assert( _sourceRow != NULL && _resultSet != NULL && _columnIds!=NULL);
		if( _resultSet->next() ) {
			_sourceRow->init(_resultSet, *_columnIds);
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
					return Iterator(_managedResults.back(), _columnIds);
				}
			case HeadIterator:
				{
					_managedResults.push_back(_headStatement->executeQuery());
					return Iterator(_managedResults.back(), _columnIds);
				}
			case FullIterator:
				{
					_managedResults.push_back(_fullStatement->executeQuery());
					return Iterator(_managedResults.back(), _columnIds);
				}
		}
	
		throw MineruleException(MR_ERROR_INTERNAL, "Uknown IteratorKind. This is a bug, please report it!");
	}
	
	SourceTable::~SourceTable() {
		for(std::vector<odbc::ResultSet*>::const_iterator it = _managedResults.begin(); it!=_managedResults.end(); ++it) {
			delete *it;
		}
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
	
	
		_bodyStatement = connection->prepareStatement( _pdu.buildBodyTableQuery(_columnIds, bodyCondition) );
		_headStatement = connection->prepareStatement( _pdu.buildHeadTableQuery(_columnIds, headCondition) );	
	}

	void SourceTable::initFullResultSet() {
		odbc::Connection* connection = MineruleOptions::getSharedOptions().getODBC().getODBCConnection();
		const OptimizedMinerule& minerule = _algorithm.optimizedMinerule();

		_fullStatement = connection->prepareStatement( _pdu.buildExtendedSourceTableQuery(_columnIds) );
	}


	void SourceTable::init() {
		if( _algorithm.needsCrossProductOfSourceTable() )
			initFullResultSet();
		else {
			initBodyHeadResultSets();
		}
	}

} // namespace