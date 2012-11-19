#ifndef SOURCETABLE_H_GE1S8NL9
#define SOURCETABLE_H_GE1S8NL9

#include "Database/SourceRow.h"
#include "Database/PrepareDataUtils.h"
#include "Algorithms/MiningAlgorithmBase.h"

namespace minerule {
	class SourceTable {
	public:
		typedef enum { BodyIterator, HeadIterator, FullIterator } IteratorKind;
		
		class Iterator {
		friend class SourceTable;			

		private:
			odbc::ResultSet* _resultSet;
			const SourceRowColumnIds* _columnIds;
			SourceRow* _sourceRow;

			Iterator( odbc::ResultSet* resultSet, const SourceRowColumnIds& columnIds) : _resultSet(resultSet), _columnIds(&columnIds), _sourceRow(new SourceRow()) {	
				assert(_resultSet!=NULL);
				if(_resultSet->isBeforeFirst()) {
					next();
				}
			}
		public:
			Iterator() : _resultSet(NULL), _columnIds(NULL), _sourceRow(NULL) {}
			Iterator( const Iterator& it ) : _resultSet(it._resultSet), _columnIds(it._columnIds), _sourceRow(it._sourceRow) {}
			virtual ~Iterator() {  /* delete _sourceRow; */ }
			
			bool next();
			bool isAfterLast() const;
			
			SourceRow* get() { assert(_sourceRow!=NULL); return _sourceRow; }
			SourceRow* operator->() { return get(); }
			
			Iterator& operator++() { next(); return *this; }			
		};
		
		
		SourceTable(const MiningAlgorithm& algorithm) : _algorithm(algorithm), _pdu(_algorithm.optimizedMinerule(), _algorithm), _bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL) { 
			init(); 
		};
		
		virtual ~SourceTable ();

		void init();
		size_t getTotGroups();
	
		Iterator newIterator(IteratorKind);
	private:
	/* data */
		const MiningAlgorithm& _algorithm;
		PrepareDataUtils _pdu;
		SourceRowColumnIds _columnIds;
		odbc::PreparedStatement* _bodyStatement;
		odbc::PreparedStatement* _headStatement;
		odbc::PreparedStatement* _fullStatement;
		
		std::vector<odbc::ResultSet*> _managedResults;
		
	/* methods */
		void initBodyHeadResultSets();	
		void initFullResultSet();	

	};
	
} // namespace	

#endif /* end of include guard: SOURCETABLE_H_GE1S8NL9 */
