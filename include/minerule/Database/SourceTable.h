#ifndef SOURCETABLE_H_GE1S8NL9
#define SOURCETABLE_H_GE1S8NL9

#include "minerule/Database/SourceRow.h"
#include "minerule/Database/PrepareDataUtils.h"
#include "minerule/Algorithms/MiningAlgorithmBase.h"

namespace minerule {
	class SourceTable {
	public:
		typedef enum { BodyIterator, HeadIterator, FullIterator } IteratorKind;
		
		class Iterator {
		friend class SourceTable;			

		private:
			odbc::ResultSet* _resultSet;
			SourceRowColumnIds _columnIds;
			SourceRow* _sourceRow;
			size_t _rowCounter;
			bool _silent;

			Iterator( odbc::ResultSet* resultSet, const SourceRowColumnIds& columnIds, bool silent ) : _resultSet(resultSet), _columnIds(columnIds), _sourceRow(new SourceRow()), _rowCounter(0), _silent(silent) {	
				
				assert(_resultSet!=NULL);
				if(_resultSet->isBeforeFirst()) {
					next();
				}
			}
		public:
			Iterator() : _resultSet(NULL), _columnIds(), _sourceRow(NULL) {}
			Iterator( const Iterator& it ) : _resultSet(it._resultSet), _sourceRow(it._sourceRow), _columnIds(it._columnIds) {
			}
			
			virtual ~Iterator() {  }
			
			bool next();
			bool isAfterLast() const;
			
			SourceRow* get() { assert(_sourceRow!=NULL); return _sourceRow; }
			SourceRow* operator->() { return get(); }
			
			Iterator& operator++() { next(); return *this; }			
		};
		
		
		SourceTable(const MiningAlgorithm& algorithm) : _minerule(algorithm.optimizedMinerule().getParsedMinerule()), _sourceTableRequirements(algorithm.sourceTableRequirements()), _pdu(_minerule, _sourceTableRequirements), _bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL), _usesCrossProduct(false) { 
			init(); 
		};
		
		SourceTable(const ParsedMinerule& minerule, const SourceTableRequirements& requirements) : _minerule(minerule), _sourceTableRequirements(requirements), _pdu(_minerule,_sourceTableRequirements), _bodyStatement(NULL), _headStatement(NULL), _fullStatement(NULL), _usesCrossProduct(false) { 
			init(); 		
		}

		virtual ~SourceTable();
		
		bool usesCrossProduct() const { return _usesCrossProduct; }

		void init();
		size_t getTotGroups();
	
		Iterator newIterator(IteratorKind);
	private:
	/* data */
		const ParsedMinerule& _minerule;
		SourceTableRequirements _sourceTableRequirements;
		PrepareDataUtils _pdu;
		bool _usesCrossProduct;
				
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
