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
#include "minerule/Database/SourceTable.hpp"
#include "minerule/PredicateUtils/HeadBodyPredicatesSeparator.hpp"
#include "minerule/mrdb/ResultSet.hpp"

namespace minerule {

	SourceTable::~SourceTable() {
		for(std::vector<mrdb::ResultSet*>::const_iterator it = _managedResults.begin(); it!=_managedResults.end(); ++it) {
			delete *it;
		}
	}

	bool SourceTable::Iterator::next() {
		++_rowCounter;
		assert( _sourceRow != NULL && _resultSet != NULL );

		if( !_silent && _rowCounter % 10000 == 0 ) {
			MRLog() << "Read " << _rowCounter << " rows so far..." << std::endl;
		}

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

					return Iterator(_managedResults.back(), bodyCols, false);
				}
			case HeadIterator:
				{
					_managedResults.push_back(_headStatement->executeQuery());
					SourceRowColumnIds headCols = _columnIds;
					headCols.bodyElems.clear();

					return Iterator(_managedResults.back(), headCols, true);
				}
			case FullIterator:
				{
					_managedResults.push_back(_fullStatement->executeQuery());
					return Iterator(_managedResults.back(), _columnIds, false);
				}
		}

		throw MineruleException(MR_ERROR_INTERNAL, "Uknown IteratorKind. This is a bug, please report it!");
	}

	size_t SourceTable::getTotGroups() {
		return _pdu.evaluateTotGroups();
	}

	void SourceTable::initBodyHeadResultSets() {
		mrdb::Connection* connection = MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection();

		std::string bodyCondition, headCondition;

		list_AND_node* miningCondition = (_minerule.mc!=NULL ? _minerule.mc->l_and : NULL);
		HeadBodyPredicatesSeparator::separate(miningCondition, bodyCondition, headCondition);

		std::string bodyQuery = _pdu.buildBodyTableQuery(_columnIds, bodyCondition);
		std::string headQuery = _pdu.buildHeadTableQuery(_columnIds, headCondition);

		_bodyStatement = connection->prepareStatement( bodyQuery );
		_headStatement = connection->prepareStatement( headQuery );

		MRLog("Body Query:"+bodyQuery);
		MRLog("Head Query:"+headQuery);
	}

	void SourceTable::initFullResultSet() {
		_usesCrossProduct = true;
		mrdb::Connection* connection = MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection();

		std::string query = _pdu.buildExtendedSourceTableQuery(_columnIds);

		MRLog("Source Table Query:"+query);
		_fullStatement = connection->prepareStatement( query );
	}


	void SourceTable::init() {
		if( _sourceTableRequirements.crossProduct() )
			initFullResultSet();
		else
			initBodyHeadResultSets();
	}

} // namespace
