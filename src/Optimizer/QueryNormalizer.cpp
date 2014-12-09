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
#include <string.h>
#include <algorithm>
#include <memory>
#include "minerule/Optimizer/QueryNormalizer.hpp"
#include "minerule/Utils/Converter.hpp"
#include "minerule/Utils/MineruleOptions.hpp"
#include "minerule/mrdb/ResultSet.hpp"
#include "minerule/mrdb/ResultSetMetaData.hpp"
#include "minerule/PredicateUtils/SimplePredAnalyzer.hpp"

namespace minerule {

    /*
     * *** PREDICATE NORMALIZATION SECTION ***
     * NOTE: The system now implements improvements !I1! and !I2!
     *       which are described below.
     *
     * (Note: numbers between '*' (e.g., *1*,*2*...), are marks used in
     * the implementation in order to specify which part of the
     * description any function implemetns).
     *
     * The following functions normalizes a predicate. Recall that the
     * predicate is in disjunctive normal form.  The normalization is
     * obtained by changing the condition in such a way that it
     * maintains the same semantics, but it uses the reference key
     * columns instead of non-reference ones.
     *
     * Let us consider a candidate substitution Ai->ai (meaning that we
     * have to substitute the set Ai of columns with the single column
     * ai) and a single disjunct having the form C0 && C1 && ... Cn
     * (each Cj is a predicate in the form a op b (where "a" and "b" are
     * attributes or values)).
     *
     * *4* The substitution is possible iff:
     *
     * [1] all elements of Ai appears in the conditions C0 ... Cn, let
     *   us call the predicates in which the elements Ai appears
     *   as G0..Gm
     *
     * [2] there are not a condition C= (a op b) in C0..Cn such that:
     *    * a and b are both attributes AND
     *    * at least one among a,b belongs to Ai
     *
     * [3] the operators are consistent with the substitution, more
     * precisely: let us call o1...om the operators in G0...Gm.
     * The substitution is possible
     * if the following conditions holds:
     *   * o1=o2..=on AND
     *   * if they are all <, or all >, the substitution is
     *     possible only if the order of the substitution is either EQUAL
     *     or REVERSED (as specified in the catalogue).
     *
     * *3* When the above conditions are met, the substitution is performed
     * as follows:
     *
     *    [1] If G0==(X0=x0), G1==(X1=x1)..Gn==(Xm=xm): let us call r a
     *    row in the source table such r(X0)=x0...r(Xm)=xm, the
     *    substitution will substitute to G0&G1&..Gn with the predicate
     *    ai=r(ai) (note that since Ai is included in {X0,..,Xm} and
     *    Ai->ai holds, then this predicate will be satisfied whenever
     *    G0&G1..&Gm is satisified).If a row r satisfying the given
     *    condition does not exist, then the substitution will be "1=0",
     *    which holds whenever G0&G1..&Gn hold (i.e.: never).
     *
     *    [2] if G0==(X0<x0), G1==(X1<x1),...,Gm==(Xm<xm):
     *    let us consider the set of R in the source table such that
     *      r(X0)<x0, r(X1)<x1, ..., r(Xm)<xm. Let us assume that R
     *    is ordered accordingly to ai and consider r=max(R) if the
     *    order between Ai and ai is EQUAL, r=min(R) otherwise).
     *    The substitution will substitute C0..Cn with
     *     -  ai<r(ai) if the order between Ai and ai is known to be EQUAL,
     *     -  ai>r(ai) otherwise
     *
     *    [3] if C0==(X0>x0)..Cn==(Xm>xm): the substitution is simmetric
     *    with respect to the * precedent case. r is chosen so that it
     *    holds r=min(R) if the ordering is EQUAL, or r=max(R)
     *    otherwise. The substitution will be:
     *     -  ai>r(ai), if the ordering is EQUAL
     *     -  ai<r(ai), otherwise.
     *
     *    [4] if G0==(X0<=X1),G1==(X1<=x1),..,Gm==(Xm<=xm): the
     *    substitution behaves as in [2], with the difference that <= is
     *    susbtituted everywhere to <
     *
     *    [5] if G0==(X0>=X1),G1==(X1>=x1),..,Gm==(Xm>=xm): the
     *    substitution behaves as in [2], with the difference that >= is
     *    susbtituted everywhere to >
     *
     * *1* If more than one conjunct is present in the predicate, the
     * procedure is repeated for each conjunct.
     *
     * *2* If more than one candidate substitution is present, all of them
     * are tried in the order in which they appear in the catalogue.
     *
     * ---------------------------------------------------------------------------
     * IMPROVEMENTS
     * ---------------------------------------------------------------------------
     * !I1! - improvement 1 (Done!)
     *
     * The algorithm for the substitution can be improved as follows:
     * For each conjunct C = X0 op x
     * find the predicate of ai that satisfies C (for instance: ai>q)
     * (take in account the order of the correspondence)
     * then put all the predicates in and.
     * A pictorial way to think to the process is the following:
     *
     *  X0>x0         X1>x1      X2>x2         q
     *  -----         -----      -----       -----
     *    |             *          |           |     q0 -> beg. of X1>x1 interv.
     *    *             *          |           |     q1 -> beg. of X0>x0 interv.
     *    *             *          |           |
     *    *             *          *           *     q2 -> beg. of X2>x2 interv.
     *    *             *          *           *     q3 -> end of X2>x2 interv.
     *    *             |          *           |
     *    *             |          *           |
     *    *             |          *           |     q4 -> end of X2>x2 interv.
     *    *             |          |           |
     *    *             |          |           |     q5 -> end of X0>x0 interv.
     *
     * the resulting predicate is: q>q2 && q<q3.
     * that is we need to find the max. beginning interval and the min ending one
     * ---------------------------------------------------------------------------
     * !I2! - improvement 2 (Done!)
     *
     * A substitution is in the form body -> head
     *
     * At the present time the algorithm which substitute the predicates tries to
     * the substitutions one at a time. It would be better to implement the same
     * schema used for the attribute substitution (i.e. we find all possible
     * substitution and then we substitute the union of the head with the union
     * of the bodies). In order to do so, the algorithm must proceed as follows:
     * 1) Find all possible substitutions (with the relative ranges)
     * 2) Remove from the predicate list, all predicate which appear in at least
     *    one substitution body
     * 3) Add all the conditions in the head of the substitutions
     *
     * ------
     * Actually the improvements above will have the effect of relaxing condition
     * *4*[3]. The new condition will be:
     *   *4*[3]New : The operator are consistent with the substitution, let o1..om
     *               be the operators found in G0..Gm. The substitution is possible
     *              iff:
     *               - o1=o2=o3..=om== '=' OR
     *               - order == REVERSED OR order == EQUAL
     *         That is: if order==NONE we must check that all operators are '=',
     *               otherwise we can always perform the substitution.
     * The second part of the improvements, make the substitution more useful, since
     * it allows a greater number of equivalences to be found.
     * ----------------------------------------------------------------------
     * !I3! - improvements 3 - DEPRECATED - It were the hint of using pre
     *        built tables in order to make the query which select min and
     *        max in the algorithm described in !I1!. It is not a good idea,
     *        since it is much simpler and more effective to use indexes on
     *        all columns.
     * ----------------------------------------------------------------------
     * !I4! - improvement 4 (Done!)
     *        Do not add predicates which are not useful. In particular:
     *        in algorithm !I1!, we should not add the predicate A<=x if
     *        x is the minimal element of column A, similarly we should not
     *        add the predicate A>=x if x is the maximal element of column A.
     * ----------------------------------------------------------------------
     * !I5! - improvement 5 (Done! implemented in SimplePredAnalyzer.cpp and in
     *        this file)
     * Compact the predicates! After the substitutions it is possible that
     * a number of predicates became redundant. Those redundancies must be
     * removed for a number of good reasons:
     *  1) The user would find them unuseful
     *  2) They threaten the possibility of find useful equivalencies. In
     *     fact we will not be able any more to identify the two predicates:
     *     (1) A<=5 and A<=100       (2) A<=5
     *     as equivalent.
     *
     * In the following we try to tackle this problem in the simplified setting
     * of dealing only with conjunction of predicates.
     * The disjunction of conjunction is handled by removing redundancies in
     * each conjunction separately and independendtly from the other conjuncts.
     * The algorithm to deal with a conjunction is the following:
     * - Select the first conjunct (this is the FIRST predicate)
     * - compare it to each of the following ones taking one of the action
     *   described below (during the comparison such predicates are referred
     *   as the SECOND predicate)
     * - select the next conjunct and repeat the process.
     *
     * when we need to eliminate a predicate we always remove the SECOND
     * predicate. When the FIRST has to be removed we swap the two predicates
     * and then we remove the SECOND.
     *
     * Unfortunately to identify and remove redundancies is not too simple
     * for a number of techninal reasons:
     *  1) The ordering of values depends on the type of the column involved.
     *     -> we must access to the DB for obtaining such information
     *  2) Many different situations may arise depending on the combination of
     *     the kind of operators involved and the ordering of values involved.
     *     Depending on the situation we could take one of the following actions:
     *          -> : the FIRST predicate implies the second one, i.e. the SECOND
     *               one is redundant and we should eliminate it
     *          <- : the SECOND predicate implies the first one.
     *          <->: the two predicates are equivalent, we should eliminate one
     *               of the two
     *           / : the two predicates are unrelated, we cannot do anything
     *           ! : the two predicates form a contraddiction. The whole
     *               and list can be discarded
     *           = : the combination of the two predicates is equivalent to
     *               one which uses one the operator '='. '=' should be subst.
     *               in one of the two predicates and the unchanged predicate
     *               should be discarded.
     *           < : as for '=', but for operator '<'
     *           > : as for '=', but for operator '>'
     *
     * Let us call X the FIRST predicate and Y the SECOND one, which action
     * should be taken depends on the following things:
     *   a) the operator Xop of predicate X
     *   b) the operator Yop of predicate Y
     *   c) the relative ordering of values appearing in X and Y (we denote
     *      with - the situation in which X value < Y value, with 0 the
     *      situation in which X value == Y value, and with + the situation
     *      in which X value > Y value.
     *
     * The following table reports for each of the 98 combinations the right
     * action to be performed.
     *
     * +-------+---+---+---+---+---+---+
     * |Xop\Yop| < | <=| = | >=| > | <>|
     * +-------+---+---+---+---+---+---+
     * |-      | ->| ->| ->| / | / | / |
     * |0    < |<->|<- | ! | ! | ! |<- |
     * |+      |<- |<- | ! | ! | ! |<- |
     * +-------+---+---+---+---+---+---+
     * |-      | ->| ->| ->| / | / | / |
     * |0   <= | ->|<->| ->| = | ! | < |
     * |+      |<- |<- | ! | ! | ! |<- |
     * +-------+---+---+---+---+---+---+
     * |-      | ! | ! | ! |<- |<- |<- |
     * |0    = | ! |<- |<->|<- | ! | ! |
     * |+      |<- |<- | ! | ! | ! |<- |
     * +-------+---+---+---+---+---+---+
     * |-      | ! | ! | ! |<- |<- |<- |
     * |0   >= | ! | = | ->|<->| ->| > |
     * |+      | / | / | ->| ->| ->| / |
     * +-------+---+---+---+---+---+---+
     * |-      | ! | ! | ! |<- |<- |<- |
     * |0    > | ! | ! | ! |<- |<->|<- |
     * |+      | / | / | ->| ->| ->| / |
     * +-------+---+---+---+---+---+---+
     * |-      | ->| ->| ->| / | / | / |
     * |0   <> | ->| < | ! | > | ->|<->|
     * |+      | / | / | ->| ->| ->| / |
     * +-------+---+---+---+---+---+---+
     *
     * ----------------------------------------------------------------------
     * !I6! Improvement 6 - we should substituted before hand all '<' predicates
     *        with '<='. More formally: Given the predicate A<x we should
     *        substitute it with A<=y where y is the first value of column A
     *        which satisfies A<x. The same have to be done for predicates A>x.
     *
     */




    // Implements *1*
    void QueryNormalizer::substituteInPredicate(
                                                list_OR_node* cond,
                                                const OptimizerCatalogue::CatalogueEntry& catEntry ) const {
        while( cond != NULL ) {
            substituteInAndList( cond->l_and, catEntry, "BODY." );
            substituteInAndList( cond->l_and, catEntry, "HEAD." );
            substituteInAndList( cond->l_and, catEntry, "" );
            compactPredicates( cond->l_and );
            cond = cond->next;
        }
    }



    // Implements *2* *3* !I2!
    void QueryNormalizer::substituteInAndList( list_AND_node*& andList,
                                              const OptimizerCatalogue::CatalogueEntry& catEntry,
                                              const std::string& prefix) const {
        BodyInfo bodies;    // information needed to delete old predicates
        HeadInfo heads;     // will be used to build new predicates

        // gathering substitution informations
        OptimizerCatalogue::CatalogueEntry::const_iterator it;
        for(it=catEntry.begin(); it!=catEntry.end(); it++) {
            findSubstitutions( andList,
                              it->first,
                              it->second.first,
                              it->second.second,
                              bodies,
                              heads,
                              prefix);
        }

        removeAndNodes( bodies );
        addNewConditions( andList, heads, prefix );
    }


    std::string QueryNormalizer::reverseOperator(std::string op, bool doReverse) const {
        if(!doReverse)
            return op;

        if( op == "<" )
            return ">";

        if( op == "<=" )
            return ">=";

        if( op == ">" )
            return "<";

        if( op == ">=" )
            return "<=";

        return op;
    }

    // Implements checking of conditions *4*[2] and *4*[3]. Building meanwhile
    // the pieces information needed for the substitution

    bool QueryNormalizer::findSubstitutions( list_AND_node*& andList,
                                            const OptimizerCatalogue::KeyCols& Ai,
                                            const OptimizerCatalogue::KeyCols& ai,
                                            const OptimizerCatalogue::OrderType order,
                                            BodyInfo& bodies,
                                            HeadInfo& heads,
                                            const std::string& prefix) const {
        if( andList == NULL )
            return false;

        list_AND_node** prevNode=&andList;
        list_AND_node* curNode=andList;
        size_t curPos = 0;

        OptimizerCatalogue::KeyCols foundCols;
        HeadInfo foundHeadEntries;
        BodyInfo foundBodyEntries;



        //
        // Note: in the following portion of code, the ill-famed "goto"
        // statement is used. I think that the code is much cleaner than
        // it would have been without it (in fact the "goto" statement is
        // used only to provide a sort of "continue" command, continue
        // cannot be use because it would skip the "curNode=curNode->next"
        // statement at the end of the while).
        while( curNode!=NULL ) {
            assert( curNode->sp != NULL );

            SubstEntryHead::Elem headElem;
            SubstEntryBody bodyEntry;
            headElem.order = order;

            if( SQLUtils::isAttribute(curNode->sp->val1) ) {
                headElem.colName = curNode->sp->val1;
                headElem.value = curNode->sp->val2;
                headElem.op = curNode->sp->op;
            }
            else if( SQLUtils::isAttribute(curNode->sp->val2) ) {
                headElem.colName = curNode->sp->val2;
                headElem.value = curNode->sp->val1;
                headElem.op = reverseOperator( curNode->sp->op );
            } else
                goto nextNode; // if neither val1, or val2 are attributes than this node is not
            // of any interest

            if( order==OptimizerCatalogue::None &&
               headElem.op!="=" ) // condition *4*[3]New
                goto nextNode;

            // We need to take in account that the mine rule may refer to single attributes
            // by prefixing them with HEAD and BODY. Since in the catalogue we do not have
            // such prefixes we need to delete them now. We need also to be sure to re-add
            // such prefixes when we add the substituted rules to the AND_list.
            size_t prefPos;

            if( prefix!="" ) {
                if( (prefPos=headElem.colName.find(prefix))!=headElem.colName.npos ) {
                    headElem.colName.erase(prefPos,5);
                } else goto nextNode;
            }

            if( Ai.find( headElem.colName )!=Ai.end() ) {
                if( SQLUtils::isAttribute( headElem.value ) ) // condition *4*[2]
                    goto nextNode;

                // All condition are met and an element of Ai found
                // inserting the proper informations in sInfo and foundCols
                bodyEntry.pred = prevNode;
                bodyEntry.pos = curPos;

                foundCols.insert( headElem.colName );
                foundBodyEntries.insert( bodyEntry );

                SubstEntryHead headEntry;
                headEntry.refKey = ai;
                HeadInfo::iterator it = foundHeadEntries.find( headEntry );

                if( it==foundHeadEntries.end() ) {
                    headEntry.elems.push_back(headElem);
                    foundHeadEntries.insert( headEntry );
                }
                else
                    const_cast< std::vector<SubstEntryHead::Elem>& >(it->elems).push_back(headElem);
            }

        nextNode:
            prevNode=&curNode->next;
            curNode=curNode->next;
            curPos++;
        }


        if( foundCols == Ai ) {
            // all Ai elements are matched in the andList.
            // We can apply the substitution.
            bodies.insert( foundBodyEntries.begin(), foundBodyEntries.end() );
            heads.insert( foundHeadEntries.begin(), foundHeadEntries.end() );

            return true;
        }

        // everything ok
        return false;
    }


    void QueryNormalizer::removeAndNodes( BodyInfo& bodies ) const {
        BodyInfo::reverse_iterator rit;
        for( rit=bodies.rbegin(); rit!=bodies.rend(); rit++ ) {
            list_AND_node* curr = *rit->pred;
            assert( curr!=NULL );
            *rit->pred = curr->next;

            delete curr->sp->val1;
            delete curr->sp->val2;
            delete curr->sp->op;
            delete curr->sp;
            delete curr;
        }
    }

    void QueryNormalizer::addNewConditions( list_AND_node*& andList,
                                           const HeadInfo& heads,
                                           const std::string& prefix) const {
        HeadInfo::const_iterator it;
        for( it=heads.begin(); it!=heads.end(); it++ ) {
            assert( it->refKey.size()==1 );
            std::string ai = *(it->refKey.begin());

            // note: the following is NOT a recursive call to this function (note
            //  that this function ends with an 's' while the following one does
            //  not).
            addNewCondition(andList, ai, it->elems, prefix);
        }
    }



    void QueryNormalizer::setSimplePred(simple_pred* pred,
                                        std::string colName,
                                        std::string op,
                                        SQLUtils::Type type,
                                        const std::string& value) const {
        std::string quote;

        if( type==SQLUtils::String )
            quote = "'";
        else
            quote = "";

        std::string val2=quote+ value + quote;

        pred->val1 = new char[colName.length()+1];
        pred->val2 = new char[val2.length()+1];
        pred->op = new char[op.length()+1];

        strcpy(pred->val1, colName.c_str() );
        strcpy(pred->op, op.c_str());
        strcpy(pred->val2, val2.c_str() );
    }

    void QueryNormalizer::addAndNode(list_AND_node*& andList,
                                     simple_pred* pred) const {
        list_AND_node* andNode = new list_AND_node;
        andNode->sp = pred;
        andNode->next = andList;
        andList = andNode;
    }



    void QueryNormalizer::addNewCondition(   list_AND_node*& andList,
                                          const std::string& aiVal,
                                          const std::vector<SubstEntryHead::Elem>& elems,
                                          const std::string& prefix) const {
        std::string queryCondition;
        std::vector<SubstEntryHead::Elem>::const_iterator it=elems.begin();
        assert(it!=elems.end());

        queryCondition = it->colName + it->op + it->value;
        it++;

        for(; it!=elems.end(); it++) {
            queryCondition += " AND " + it->colName + it->op + it->value;
        }

        std::string query = (std::string)
        "SELECT " +   " min(" + aiVal + "), max(" + aiVal +")"
        " FROM " + mr->tab_source +
        " WHERE " + queryCondition; // +
        // " ORDER BY " +aiVal;

        mrdb::Connection* con =
        MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection();
        mrdb::Statement* state=NULL;
        mrdb::ResultSet* rs=NULL;

        simple_pred* lower_bound  = NULL;
        simple_pred* upper_bound  = NULL;

        try {
            state = con->createStatement();
            rs = state->executeQuery(query);

            if( ! rs->next() ) {
                lower_bound = new simple_pred;
                lower_bound->val1 = new char[2];
                lower_bound->val2 = new char[2];
                lower_bound->op = new char[2];

                strcpy( lower_bound->val1, "0" );
                strcpy( lower_bound->val2, "1" );
                strcpy( lower_bound->op, "=");
            } else if( rs->getString(1)==rs->getString(2) ) {
                lower_bound = new simple_pred;
                std::string val = rs->getString(1);
                setSimplePred( lower_bound, prefix+aiVal, "=", SQLUtils::getType(rs, 1), val);
            } else {
                std::string minVal=rs->getString(1);
                std::string maxVal=rs->getString(2);

                delete rs;
                delete state;
                rs=NULL;
                state=NULL;

                query = (std::string)
                "SELECT " +   " min(" + aiVal + "), max(" + aiVal +")"
                " FROM " + mr->tab_source;

                state = con->createStatement();
                rs = state->executeQuery(query);

                assert( rs->next() );

                std::string new_aiVal = prefix+aiVal;

                if( rs->getString(1)!=minVal ) {
                    lower_bound = new simple_pred;
                    setSimplePred( lower_bound, new_aiVal, ">=", SQLUtils::getType(rs,1), minVal );
                }

                if( rs->getString(2)!=maxVal ) {
                    upper_bound = new simple_pred;
                    setSimplePred( upper_bound, new_aiVal, "<=", SQLUtils::getType(rs,2), maxVal);
                }
            }

            if( lower_bound!=NULL )
                addAndNode( andList, lower_bound );

            if( upper_bound!=NULL )
                addAndNode( andList, upper_bound );
        } catch (...) {
            if( rs != NULL ) delete rs;
            if( state!=NULL ) delete state;
            if( lower_bound!=NULL ) delete lower_bound;
            if( upper_bound!=NULL ) delete upper_bound;
            throw;
        }

        delete rs;
        delete state;
    }


    /*
     *  *** END (PREDICATE NORMALIZATION SECTION) ***
     */



    /**
     * This function normalizes l using the substitutions specified in
     * the catalogue.  If A0->a0 ... An->an are in the catalogue, then
     * the set of admissible ones is given by S={ Ai->ai | Ai
     * is_included_in l } (we assume that each Ai is a set of column
     * names, while each ai is a single column name). Let us call
     * SHead=\Union Ai, Ai->x belongs to S.  And STail=\Union ai, x->ai
     * belongs to S. The normalized l is defined as (l-SHead) Union
     * Stail
     */
    void QueryNormalizer::substituteInAttrList( ParsedMinerule::AttrVector& l,
                                               const OptimizerCatalogue::CatalogueEntry& catEntry
                                               ) const {
        // each element of catEntry is a relation A->B, we call A the head and B the tail
#define HEAD(a) ((a).first)
#define TAIL(a) ((a).second.first)

        // just to be sure that l is sorted...
        sort(l.begin(),l.end());

        std::set<std::string> heads; // <- union of all heads elements
        std::set<std::string> tails; // <- union of all tails elements

        OptimizerCatalogue::CatalogueEntry::const_iterator eit;

        // tails  <- union of all tails in the current entry of the catalog which can be applied to l
        for(eit=catEntry.begin(); eit!=catEntry.end(); eit++ ) {
            if( TAIL(*eit).size() != 1 ) {
                MRErr() << "Warning: Found a substitution A->B, whit |B|<>0, it will be ignored!"<<std::endl;
                continue;
            }

            // -- looking if the current head is contained in l
            OptimizerCatalogue::KeyCols::const_iterator hit;
            for( hit=HEAD(*eit).begin();
                hit!=HEAD(*eit).end()  && find(l.begin(),l.end(),*hit)!=l.end();
                hit++ ) /* noop */ ;

            if( hit!=HEAD(*eit).end() ) // if(not all head elems can be found in l)
                continue;                 // skip; i.e., look for another head

            // -- adding HEAD and TAIL to the set of found heads and tails elements
            // insert all head elements in "heads"
            copy( HEAD(*eit).begin(), HEAD(*eit).end(),
                 std::insert_iterator< std::set<std::string> >(heads, heads.begin()) );

            // insert the single tail element in tails
            tails.insert( *TAIL(*eit).begin() );
        }


        // Now that we have all the relevant informations about all the rules that can
        // be applied, we can perform the substitution

        std::vector<std::string> tmp;
        // tmp = l - heads;
        set_difference( l.begin(), l.end(),
                       heads.begin(), heads.end(),
                       std::insert_iterator< std::vector<std::string> >(tmp,tmp.begin()));
        // l = Union( tmp, tails)
        l.clear();
        set_union( tmp.begin(), tmp.end(),
                  tails.begin(), tails.end(),
                  std::insert_iterator< ParsedMinerule::AttrVector >( l, l.begin() ) );

#undef HEAD
#undef TAIL
    }

    // Substitutes < with <= and > with >=
    void QueryNormalizer::relaxOperators(list_OR_node* cond)
    throw(MineruleException,mrdb::SQLException) {
        while( cond!=NULL ) {
            relaxOperatorsInAndList(cond->l_and);
            cond=cond->next;
        }
    }

    std::string QueryNormalizer::getRelaxedValue(const std::string& tabSource,
                                                 const std::string& attr,
                                                 const std::string& op,
                                                 const std::string& value)
    throw(MineruleException,mrdb::SQLException) {
        assert(op=="<" || op==">");

        mrdb::Connection* conn =
        MineruleOptions::getSharedOptions().getMRDB().getMRDBConnection();
        std::auto_ptr<mrdb::Statement> state(conn->createStatement());

        std::string sqlfun;
        if( op=="<" )
            sqlfun="max";
        if( op==">" )
            sqlfun="min";

        std::string query =
        "SELECT "+sqlfun+"("+attr+") "
        "FROM "+tabSource+" "
        "WHERE "+attr+op+value;

        std::auto_ptr<mrdb::ResultSet> result(state->executeQuery(query));
        if(!result->next())
            throw MineruleException(MR_ERROR_DATABASE_ERROR,
                                    "Failed to select "+sqlfun+" of column "+attr+
                                    " of table "+tabSource);

        return SQLUtils::quote(result->getString(1));
    }

    void QueryNormalizer::relaxOperatorInPred(simple_pred* pred)
    throw(MineruleException,mrdb::SQLException) {
        std::string attribute;
        std::string op;
        std::string value;
        std::string newvalue;
        std::string prefix;
        size_t prefPos;

        if( SQLUtils::isAttribute(pred->val1) ) {
            attribute=pred->val1;
            value = pred->val2;
            op = pred->op;
        }
        else {
            attribute=pred->val2;
            value = pred->val1;
            op = reverseOperator(pred->op);
        }

        assert(op=="<" || op==">");

        // we do not handle cross predicates (i.e. A<B where both A and B are
        // attributes).
        if(SQLUtils::isAttribute(value))
            return;

        if( (prefPos=attribute.find("HEAD."))!=attribute.npos ) {
            attribute.erase(prefPos,5);
            prefix="HEAD.";
        }

        if( (prefPos=attribute.find("BODY."))!=attribute.npos ) {
            assert(prefix=="");
            attribute.erase(prefPos,5);
            prefix="BODY.";
        }

        newvalue = getRelaxedValue(mr->tab_source, attribute, op, value);

        // we now have all the info we can make the substitutions

        // trashing the trashable
        delete pred->val1;
        delete pred->op;
        delete pred->val2;

        attribute=prefix+attribute;

        // setting val1 (i.e. attribute name)
        pred->val1=new char[attribute.size()+1];
        strcpy( pred->val1, attribute.c_str() );
        // setting op
        pred->op = new char[3];
        pred->op[0]=op[0];
        pred->op[1]='=';
        pred->op[2]='\0';
        // setting val2 (i.e. newname);
        pred->val2=new char[newvalue.size()+1];
        strcpy( pred->val2, newvalue.c_str() );
    }

    void QueryNormalizer::relaxOperatorsInAndList(list_AND_node* andlist)
    throw(MineruleException,mrdb::SQLException) {
        while(andlist!=NULL) {
            char* op=andlist->sp->op;
            if(!strcmp(op,"<") || !strcmp(op,">"))
                relaxOperatorInPred(andlist->sp);

            andlist=andlist->next;
        }
    }

    void QueryNormalizer::cleanPredicate(list_OR_node*& cur) const {
        if(cur==NULL)
            return;

        if( cur->l_and==NULL ) {
            list_OR_node* old_node=cur;
            cur=cur->next;
            delete old_node;
            cleanPredicate(cur);
        } else {
            cleanPredicate(cur->next);
        }
    }

    /*
     In this procedure will be contained all the logic for making safe
     substitutions. At the present time it only handles A = B mappings
     (with A and B being single attributes).
     */
    void QueryNormalizer::normalize() throw(MineruleException,mrdb::SQLException) {
        OptimizerCatalogue::Catalogue::const_iterator it = catalogue.find( mr->tab_source );
        if( it==catalogue.end() )
            return;  // no mapping info for this table

        substituteInAttrList( mr->ga, it->second );
        substituteInAttrList( mr->ca, it->second );
        substituteInAttrList( mr->ra, it->second );
        substituteInAttrList( mr->ba, it->second );
        substituteInAttrList( mr->ha, it->second );

        substituteInPredicate( mr->mc, it->second );
        substituteInPredicate( mr->gc, it->second );
        substituteInPredicate( mr->cc, it->second );
        relaxOperators(mr->mc);
        relaxOperators(mr->gc);
        relaxOperators(mr->cc);

        // We may have removed a number of l_and lists
        // an l_or node having an empty l_and list is an
        // invalid node, we need to clear them!
        cleanPredicate(mr->mc);
        cleanPredicate(mr->gc);
        cleanPredicate(mr->cc);
    }


    //  ----------------------------------------------------------------------
    //  COMPACT PREDICATE AND HELPERS
    //  ----------------------------------------------------------------------

    void removeNode( list_AND_node** l ) {
        list_AND_node* node = *l;
        *l = node->next;
        delete node->sp->val1;
        delete node->sp->val2;
        delete node->sp->op;
        delete node->sp;
        delete node;
    }

    void removeAllNodes( list_AND_node*& l ) {
        while( l!=NULL ) {
            removeNode(&l);
        }
    }



    // it implements !I5!.

    void QueryNormalizer::compactPredicates( list_AND_node*& l ) const {
        list_AND_node** Xptr;

        for( Xptr=&l; *Xptr!=NULL; Xptr = &((*Xptr)->next) ) {
            char *Xattr,*Xvalue;

            bool XreverseOp;
            if(!SimplePredAnalyzer::isAttrOpValuePredicate((*Xptr)->sp,Xattr,Xvalue,XreverseOp))
                continue;

            list_AND_node** Yptr;
            list_AND_node* oldCell;

            // note the strange increment in the thir for argument.
            // We need to advance only when we did not delete the cell
            // pointed by Yptr. In fact, in this situation the pointer
            // must stay the same (althought the cell it points to has
            // been changed by the remove operator).
            for( Yptr=&(*Xptr)->next; *Yptr!=NULL; *Yptr==oldCell ? (Yptr=&(*Yptr)->next) : 0 ) {
                oldCell=*Yptr;

                char *Yattr,*Yvalue;
                bool YreverseOp;
                if(!SimplePredAnalyzer::isAttrOpValuePredicate((*Yptr)->sp,
                                                               Yattr,Yvalue,
                                                               YreverseOp))
                    continue;

                if(strcmp(Xattr,Yattr))
                    continue;

                SQLUtils::Type type =
                SQLUtils::getType( MineruleOptions::getSharedOptions()
                                  .getMRDB()
                                  .getMRDBConnection(),
                                  mr->tab_source, Xattr);

                char opRel;
                switch( opRel=SimplePredAnalyzer::getRelation(
                                                              reverseOperator((*Xptr)->sp->op,XreverseOp).c_str(), Xvalue,
                                                              reverseOperator((*Yptr)->sp->op,YreverseOp).c_str(), Yvalue,
                                                              type) ) {
                    case '/': // do nothing simply skip to next node
                        break;
                    case '!': // clean the main list and return
                        removeAllNodes(l);
                        return;
                    case '=': // substitute opRel in X and delete Y
                        (*Xptr)->sp->op[0]='=';
                        (*Xptr)->sp->op[1]='\0';
                        removeNode(Yptr);
                        break;
                    case '<':
                    case '>':
                        char buf[2];
                        buf[0]=opRel;
                        buf[1]='\0';
                        if(!strcmp((*Xptr)->sp->op,"<>"))
                            (*Xptr)->sp->op[0]=reverseOperator(buf,YreverseOp)[0];
                        else if(!strcmp((*Yptr)->sp->op,"<>"))
                            (*Xptr)->sp->op[0]=reverseOperator(buf,XreverseOp)[0];
                        else throw MineruleException( MR_ERROR_INTERNAL, "The optimizer found a strange operators configuration..."
                                                     "this is a bug!");
                        (*Xptr)->sp->op[1]='\0';
                        removeNode(Yptr);
                        //	  nodeToRemove = Yptr;
                        break;
                    case 'l': {// substitute Y to X, delete X
                        simple_pred* tmp = (*Yptr)->sp;
                        (*Yptr)->sp=(*Xptr)->sp;
                        (*Xptr)->sp=tmp;
                        removeNode(Yptr);
                        //	  nodeToRemove = Yptr;
                    }
                        break;
                    case 'r': // delete Y
                    case 'b': // delete Y
                        removeNode(Yptr);
                        //	  nodeToRemove = Yptr;
                        break;
                    default:
                        throw MineruleException(MR_ERROR_OPTIMIZER_ERROR,
                                                "Unknown relationship between two operators"
                                                " this is a BUG!");
                }
            }

        }
    }




} // namespace
