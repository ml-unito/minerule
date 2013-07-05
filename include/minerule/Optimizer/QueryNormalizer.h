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
#ifndef __QUERYNORMALIZER_H__
#define __QUERYNORMALIZER_H__

#include <vector>
#include "minerule/Parsers/ParsedMinerule.h"
#include "minerule/Optimizer/OptimizerCatalogue.h"
#include "minerule/Utils/SQLUtils.h"

namespace minerule { 

  /* 
   * Allows to normalize ParsedMinerules so that they are more easily
   * checked for equivalence,dominance and inclusion relationships.
   * Look at the implementation file for more comments about how it
   * actually works.
   */

  class QueryNormalizer {
  public:
    /* ---------- Public Types ---------- */
    /* ---------- (needed for query normalization) ---------- */
    class SubstEntryBody {
    public:
      // pred is the ptr to the 'next' field of the node preceding
      // the node from which this struct were built
      // (note that it can be used to unlink the current
      // node).
      list_AND_node** pred;
      size_t pos;

      SubstEntryBody() {
	pred=NULL;
	pos=0;
      }

      SubstEntryBody(const SubstEntryBody& seb) :
	pred( seb.pred ),
	pos( seb.pos ) { }

      bool operator<(const SubstEntryBody& s2) const {
	return this->pos < s2.pos;
      } 
    };
    
    class SubstEntryHead {
    public:

      class Elem {
      public:
	OptimizerCatalogue::OrderType  order;
      
std::string colName;        // the column name
std::string value;          // the adjoint value
std::string op;             // the operator involved

	Elem() {
	}
	
	Elem(const Elem& elem) : 
	  order(elem.order),
	  colName(elem.colName),
	  value(elem.value),
	  op(elem.op)  {}
      }; // class Elem

      OptimizerCatalogue::KeyCols        refKey;
      std::vector<Elem>   elems;
      
      SubstEntryHead() {
      }

      SubstEntryHead(const SubstEntryHead& seh) :
	refKey( seh.refKey ),
	elems( seh.elems ) {}

      bool operator<(const SubstEntryHead& s2) const {
	return this->refKey<s2.refKey;
      }
    };
  
    typedef std::set<SubstEntryHead> HeadInfo;
    typedef std::set<SubstEntryBody> BodyInfo;
  private:
       /* ---------- Member Fields ---------- */
    const OptimizerCatalogue& optimizerCatalogue;
    const OptimizerCatalogue::Catalogue& catalogue;
    ParsedMinerule* mr;


    /* ---------- Private Methods ---------- */
   std::string reverseOperator(std::string op, bool doReverse=true ) const;

    void substituteInPredicate( 
		list_OR_node* cond,
		const OptimizerCatalogue::CatalogueEntry& catEntry ) const;

    void substituteInAndList(
		list_AND_node*& andlist,
		const OptimizerCatalogue::CatalogueEntry& catEntry,
		const std::string& prefix) const;

    void performSubstitutionInAndList( 
		list_AND_node*& andList,
		const OptimizerCatalogue::KeyCols& Ai,
		const OptimizerCatalogue::KeyCols& ai,
		OptimizerCatalogue::OrderType order,
		const std::string& prefix) const;

    bool findSubstitutions( list_AND_node*& andList,
			    const OptimizerCatalogue::KeyCols& Ai,
			    const OptimizerCatalogue::KeyCols& ai,
			    const OptimizerCatalogue::OrderType order,
			    BodyInfo& bodies,
			    HeadInfo& heads,
			    const std::string& prefix) const;


    void setSimplePred(simple_pred* pred,
		      std::string colName,
		      std::string op,
		       SQLUtils::Type,
		       const std::string& ) const;

    void removeAndNodes( BodyInfo& sInfo ) const;
    void addAndNode(list_AND_node*& andList,
		    simple_pred* pred) const;

    void addNewConditions( list_AND_node*& andList,
			   const HeadInfo& heads,
			   const std::string& prefix) const;

    void addNewCondition( list_AND_node*& andList,
			  const std::string& aiVal,
			  const std::vector<SubstEntryHead::Elem>& elems,
			  const std::string& prefix) const;

    void substituteInAttrList( 
                ParsedMinerule::AttrVector& l,
		const OptimizerCatalogue::CatalogueEntry& catEntry
		) const;

    void compactPredicates( list_AND_node*& l ) const;
    
    void relaxOperatorsInAndList(list_AND_node* andlist) 
      throw(MineruleException,odbc::SQLException);

    void relaxOperatorInPred(simple_pred* pred) 
      throw(MineruleException,odbc::SQLException);

   std::string getRelaxedValue(const std::string& tabSource,
			   const std::string& attr,
			   const std::string& op,
			   const std::string& value) 
      throw(MineruleException,odbc::SQLException);

    void relaxOperators(list_OR_node* cond) 
      throw(MineruleException,odbc::SQLException);

    void cleanPredicate(list_OR_node*& cur) const;
  public:
    /* ---------- Public Methods ---------- */

    QueryNormalizer( const OptimizerCatalogue& oc, 
		     ParsedMinerule& mrule ) : optimizerCatalogue(oc), 
                                               catalogue( oc.getCatalogue() ),
					       mr(&mrule) {
    };

    

    void setMinerule( ParsedMinerule& mrule ) {
      mr = &mrule;
    }

    // As a side effect it normalizes the minerule mr
    void normalize() throw(MineruleException,odbc::SQLException);
  };

} // namespace

#endif
