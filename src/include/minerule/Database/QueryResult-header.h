#ifndef __QUERY_RESULT_HEADER_H__
#define __QUERY_RESULT_HEADER_H__

#include "Database/ItemType.h"
#include "Parsers/ParsedMinerule.h"
#include "odbc++/preparedstatement.h"
#include "Database/Rule.h"

namespace minerule {

	class QueryResult {
	public:
		
	// ----------------------------------------------------------------------
	// Rule
	// ----------------------------------------------------------------------
		
		// --------------------------------------------------------------------------------
		// Iterator 
		// --------------------------------------------------------------------------------


		class Iterator {
		private:
			odbc::Statement* state;
			odbc::ResultSet* rs_rules;
			odbc::PreparedStatement* body_elems;
			odbc::PreparedStatement* head_elems;

			void readElems(int id, ItemSet& elems, odbc::PreparedStatement* elems_rs)
				throw ( MineruleException, odbc::SQLException, std::exception);
		public:
			Iterator() : state(NULL), rs_rules(NULL), body_elems(NULL), head_elems(NULL) {}

			~Iterator() {
				if(rs_rules!=NULL)
					delete rs_rules;

				if(state!=NULL)
					delete state;

				if( body_elems!=NULL ) 
					delete body_elems;

				if( head_elems!=NULL ) 
					delete head_elems;
			}


	      //
		  // initialize the result set
	      //
			void init( const std::string& rulesTable, double support, double confidence ) throw( MineruleException, odbc::SQLException );

		  // it moves the iterator on the following rule. The first time it is called
		  // it position on the first element. It return false when it cannot
		  // succeed in positioning on the next rule.
			bool next();
  
	      //
		  // It fills the Rule given as input with the current 
		  // values.
	      //
     
			void getRule( Rule& r ) throw( MineruleException, odbc::SQLException, std::exception );
		};


		// --------------------------------------------------------------------------------
		// SortBodyHeadSuppConf 
		// --------------------------------------------------------------------------------

		class SortBodyHeadSuppConf {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if( *r1.body > *r2.body ) 
					return true;
	
				if( *r1.body == *r2.body ) {
					if(*r1.head > *r2.head )
						return true;
	  
					if(*r1.head == *r2.head) {
						return r1.support > r2.support || 
							(r1.support==r2.support && r1.confidence > r2.confidence);
					}
				}

				return false;
			}
		};


		// --------------------------------------------------------------------------------
		// SortBodyHead 
		// --------------------------------------------------------------------------------
    
		class SortBodyHead {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if( *r1.body > *r2.body ) 
					return true;
	
				if( *r1.body == *r2.body ) {
					if(*r1.head > *r2.head )
						return true;
				}

				return false;
			}
		};


		// --------------------------------------------------------------------------------
		// SortHeadBodySuppConf 
		// --------------------------------------------------------------------------------

		class SortHeadBodySuppConf {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if( *r1.head > *r2.head ) 
					return true;
	
				if( *r1.head == *r2.head ) {
					if(*r1.body > *r2.body )
						return true;
	  
					if(*r1.body == *r2.body) {
						return r1.support > r2.support || 
							(r1.support==r2.support && r1.confidence > r2.confidence);
					}
				}
	
				return false;
			}
		};


		// --------------------------------------------------------------------------------
		// SortSuppConf 
		// --------------------------------------------------------------------------------
    
		class SortSuppConfBodyHead {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if(r1.support > r2.support)
					return true;
				if(r1.support < r2.support)
					return false;

	// here r1.support == r2.support
				if(r1.confidence > r2.confidence)
					return true;
				if(r1.confidence < r2.confidence)
					return false;

	// here r1.sup=r2.sup && r1.conf=r2.conf
				if(*r1.body > *r2.body)
					return true;
				if(*r1.body < *r2.body)
					return false;

				if(*r1.head > *r2.head)
					return true;
				if(*r1.head < *r2.head)
					return false;

	// here all fields are equals
				return false;
			}
		};

		// --------------------------------------------------------------------------------
		// SortConfSuppBodyHead 
		// --------------------------------------------------------------------------------


		class SortConfSuppBodyHead {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if(r1.confidence > r2.confidence)
					return true;
				if(r1.confidence < r2.confidence)
					return false;

				if(r1.support > r2.support)
					return true;
				if(r1.support < r2.support)
					return false;

				if(*r1.body > *r2.body)
					return true;
				if(*r1.body < *r2.body)
					return false;

				if(*r1.head > *r2.head)
					return true;
				if(*r1.head < *r2.head)
					return false;

	// here all fields are equals
				return false;
			}
		};

		// --------------------------------------------------------------------------------
		// SortConfBodyHeadSupp 
		// --------------------------------------------------------------------------------


		class SortConfBodyHeadSupp {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if(r1.confidence > r2.confidence)
					return true;
				if(r1.confidence < r2.confidence)
					return false;
	
				if(*r1.body > *r2.body)
					return true;
				if(*r1.body < *r2.body)
					return false;

				if(*r1.head > *r2.head)
					return true;
				if(*r1.head < *r2.head)
					return false;

				if(r1.support > r2.support)
					return true;
				if(r1.support < r2.support)
					return false;

				return false;
			}
		};

		// --------------------------------------------------------------------------------
		// SortConfBodySuppHead 
		// --------------------------------------------------------------------------------


		class SortConfBodySuppHead {
		public:
			bool operator()(const Rule& r1, const Rule& r2) const {
				if(r1.confidence > r2.confidence)
					return true;
				if(r1.confidence < r2.confidence)
					return false;

				if(*r1.body > *r2.body)
					return true;
				if(*r1.body < *r2.body)
					return false;

				if(r1.support > r2.support)
					return true;
				if(r1.support < r2.support)
					return false;

				if(*r1.head > *r2.head)
					return true;
				if(*r1.head < *r2.head)
					return false;

	// here all fields are equals
				return false;
			}
		};



	// --------------------------------------------------------------------------------
	// ResultSet fields and methods (at last ;-)) 
	// --------------------------------------------------------------------------------

	// This class allows one to load in memory a whole result set.
	// It also allows to perform intersections and unions of different
	// query results, as well as to sort the result using different 
	// criteria.	

    template <class Sorter>
		class ResultSet : public  std::set<Rule,Sorter> {     

		public:
			ResultSet() {}
			ResultSet(const ResultSet<Sorter>& rhs) {}
    
      
	  	// Load a result set into the object. NOTICE: this function (this class
	  	// at large) should be used when you need to load a full result set
	  	// into memory. Do not use it as a short-hand for iterating through
	  	// a result set. Use the ResultSetIterator class instead (you can
	  	// initialize it using the OptimizerCatalogue. Another other reason
	  	// that justify the use of this class is when you need to iterate thru
	  	// the result set, but you require it to be sorted).
	  	// 
	  	// @param qryName the name of the query to load into the class
	  	// @param sup Support threshold used to filter out uninteresting rules.
	  	// The default is -1 and it means that no filter should occur.
	  	// @param conf Confidence threshold used to filter out uninteresting rules.
	  	// The default is -1 and it means that no filter should occur.

			void load(const std::string& qryName, double sup=-1, double con=-1) throw(MineruleException);
    
		//
		// Save the current result set. The tables being created are associated
		// to the minerule mr. Notice that this function do not update the catalogue.
		//

			void save(const ParsedMinerule& mr) const;

			ResultSet& inplace_intersect(const ResultSet&);
			ResultSet& inplace_union(const ResultSet&);

		// 
		// It builds the intersection of rhs and *this. The result is stored
		// into *this. 
		// Notice that whether two rules has to be considered equal depends
		// on the Sorter you choose. For instance if you use the SortSuppConf
		// sorter, then two queries are equal iff 
		// 
		// @param rhs the query that has to be intersected with *this
		// 
			ResultSet& operator&=(const ResultSet&);

		// 
		// It builds the union of rhs and *this. The result is stored
		// into *this.
		// 
		// @param rhs the query that has to be merged with *this
		// 
			ResultSet& operator|=(const ResultSet&);

		}; // ResultSet

	}; // QueryResult
} // namespace

#endif
