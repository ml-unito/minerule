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
			SourceRow* _sourceRow;
			const SourceRowColumnIds& _columnIds;
			odbc::ResultSet* _resultSet;

			Iterator( odbc::ResultSet* resultSet, const SourceRowColumnIds& columnIds) : _resultSet(resultSet), _columnIds(columnIds) {
				_sourceRow = new SourceRow(_resultSet, _columnIds);
			}

		public:			
			virtual ~Iterator() { delete _sourceRow; }
			
			bool next();
			bool isAfterLast() const;
			
			SourceRow* get() { assert(_sourceRow!=NULL); return _sourceRow; }
			SourceRow& operator->() { return *get(); }
			
			void operator++() { next(); }			
		};
		
		
		SourceTable(const MiningAlgorithm& algorithm) : _algorithm(algorithm), _pdu(_algorithm.optimizedMinerule(), _algorithm), _bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL) { 
			init(); 
		};
		
		virtual ~SourceTable ();

		void init();
		size_t getTotGroups();
	
		Iterator* newIterator(IteratorKind);
	private:
	/* data */
		const MiningAlgorithm& _algorithm;
		PrepareDataUtils _pdu;
		SourceRowColumnIds _columnIds;
		odbc::PreparedStatement* _bodyStatement;
		odbc::PreparedStatement* _headStatement;
		odbc::PreparedStatement* _fullStatement;
		
	/* methods */
		void initBodyHeadResultSets();	
		void initFullResultSet();	

	};
	
} // namespace	

#endif /* end of include guard: SOURCETABLE_H_GE1S8NL9 */
