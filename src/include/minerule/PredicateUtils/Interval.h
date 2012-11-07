#ifndef __INTERVAL_CHECKER__
#define __INTERVAL_CHECKER__

#include <string.h>
#include "Utils/MineruleException.h"
#include "odbc++/types.h"
#include <set>
#include <map>
#include <string>
#include "PredicateUtils/VarSet.h"
#include "Parsers/ParsedMinerule.h"
#include "Utils/SQLUtils.h"


namespace minerule {

  /* ======================================================================
   * StringCompare
   * ====================================================================== */
  class StringCompare {
  public:
    bool operator()(const char* lhs, const char* rhs) const {
      return strcmp(lhs,rhs)<0;
    }
  };


  /* ======================================================================
   * Interval
   * ======================================================================
   */

  class Interval {
    friend std::ostream& operator<<(std::ostream&, const Interval&);
  public:
    /**
     * An enumerated type corresponding to the operators <,<=,>,>=,=,<>
     */ 
    typedef enum {
      Less,
      LessEq,
      Grt,
      GrtEq,
      Eq,
      NotEq
    } Operator;

  protected:
    /**
     * Thestd::string used to represent positive infty. Notice that its
     * content are not really important since, all the comparison will
     * be made using the address of the string.
     */
    static const char* POSINFTY;
    /**
     * Thestd::string used to represent negative infty. Notice that its
     * content are not really important since, all the comparison will
     * be made using the address of the string.
     */
    static const char* NEGINFTY;

    /**
     * The lower part of the interval (e.g., if the object is used to
     * represent (10,4], then lwr will contain thestd::string "10").
     */
    const char* lwr; // lower limit
    /**
     * The upper part of the interval (e.g., if the object is used to
     * represent (10,4], then upp will contain thestd::string "4").
     */
    const char* upp; // upper limit
    /**
     * true if the interval is open on the left (e.g., if the object
     * is used to represent (10,4], then lwrOpen is set to true)
     */
    bool lwrOpen;
    
    /**
     * true if the interval is open on the right (e.g., if the object
     * is used to represent (10,4], then uppOpen is set to false)
     */
    bool uppOpen;

    /**
     * The odbc type of the attribute corresponding to this
     * interval. It is used in order to compare values in all the
     * the operations.
     */
    SQLUtils::Type type;
    /**
     * Used to keep track of whether type has been already setted or
     * not.
     */
    bool typeOk;

    /**
     * Used to keep tracks of isolated points that must not be considered
     * to be valid ones (they corresponds to <> predicates)
     */
    std::set<const char*, StringCompare> openPoints;


    /**
     * @param op Astd::string representing a supported operator (that is,
     * one of "<,<=,>,>=,=,<>") 
     *
     * @param negateIt If true, the function returns the symbolic
     * representation of the negated operator 
     *
     * Returns the symbolic representation of the operator described
     * by op. If negateIt is true, the results corresponds to the
     * negation of the operator, e.g., getOperator("<=",false) returns
     * LessEq; getOperator("<=",true) returns Grt;
     */
    Operator getOperator( const char* op, bool negateIt ) const throw(MineruleException);

    /**
     * Returns -1 if val1<val2 0 if val1=val2 and 1 if val1>val2. The comparison is taken w.r.t. the
     * type reported by the "type" member variable.
     */
    int compareValues( const char* val1, const char* val2) const throw(MineruleException);
    
    /**
     * @param lhs an Interval
     * @param rhs an Interval
     * @param maxIsOpen a boolean used as an additional return value
     *
     * Returns the highest between lhs.lwr and rhs.lwr, in addition it sets maxIsOpen accordingly to
     * whether the corresponding interval is open on the left or not. Example: let us denote an Interval
     * with the usual mathematical notation. getMinUpp( (10,20], [11,15) ) returns 11 since 11 is the highest
     * lwr value, and sets maxIsOpen=false since [11,15) is closed on the left side.
     */
    const char* getMaxLwr( Interval& lhs, Interval& rhs, bool& maxIsOpen ) const throw(MineruleException);

    /**
     * @param lhs an Interval
     * @param rhs an Interval
     * @param minIsOpen a boolean used as an additional return value
     *
     * Returns the lowest between lhs.upp and rhs.upp, in addition it sets minIsOpen accordingly to
     * whether the corresponding interval is open on the right or not. Example: let us denote an Interval
     * with the usual mathematical notation. getMinUpp( (10,20], [11,15) ) returns 15 since 15 is the lowest
     * upp value, and sets minIsOpen=true since [11,15) is open on the right side.
     */
    const char* getMinUpp( Interval& lhs, Interval& rhs, bool& minIsOpen ) const  throw(MineruleException);


  public:
    Interval() : lwr(NEGINFTY), upp(POSINFTY), lwrOpen(true), uppOpen(true), typeOk(false) { }
    Interval(const Interval& rhs) : 
      lwr(rhs.lwr), 
      upp(rhs.upp), 
      lwrOpen(rhs.lwrOpen), 
      uppOpen(rhs.uppOpen),
      type(rhs.type),
      typeOk(rhs.typeOk) {}
    Interval( Operator op, const char* val, SQLUtils::Type t );


    /**
     * @param i An Interval
     * Intersects the current interval with the i.
     */
    void intersect(Interval& i) {
      lwr = getMaxLwr(*this,i,lwrOpen);
      upp = getMinUpp(*this,i,uppOpen);
      openPoints.insert( i.openPoints.begin(), i.openPoints.end() );
    }
    
    
    /**
     * Intersects the current interval with the one built from l->sp
     */
    void update(const list_AND_node* l, bool negateIt) throw(MineruleException);


    /**
     * It sets type = t and typeOk to true;
     */
    void setType( SQLUtils::Type t ) {
      type=t;
      typeOk = true;
    }


    /**
     * Returns true if the current interval is an empty one.
     */
    bool isEmpty() {
      int order = compareValues(lwr,upp);
      if( order < 0 )  return false;
      if( order == 0 ) return lwrOpen || uppOpen || (openPoints.find(lwr)!=openPoints.end());
      return true;
    }


    /**
     * @param val1
     * @param val2
     *
     * Returns one among val1 and val2. In particular it returns the
     * first of the two which is found not to be an attribute
     * identifier. For instance, if val1=="A" val2=="5" the function
     * returns "5".
     */
    static const char* getValue( const char* val1, const char* val2 );


    /**
     * @param val1
     * @param val2
     *
     * Returns one among val1 and val2. In particular it returns the
     * first of the two which is found to be an attribute
     * identifier. For instance, if val1=="A" val2=="5" the function
     * returns "A".
     */
    static const char* getAttribute(const char* val1, const char* val2 );
  };

  std::ostream& operator<<(std::ostream&, const Interval&);
  
  /* ======================================================================
   * IntervalChecker
   * ======================================================================
   */


  class IntervalChecker {
    std::map<const char*, SQLUtils::Type> typesMap;
    const std::string& tab_source;
    
  public:
    IntervalChecker( const std::string& t ) : tab_source(t) {}

    /**
     * @param attr
     *
     * It returns the type for attribute attr by searching the map
     * typesMap. In case the type cannot be found in typesMap, then it
     * try to determine it using SQLUtils and update the map
     * accordingly. If it cannot determine the type, it throw an
     * exception.
     */
    SQLUtils::Type typeForAttribute(const char* attr) throw(MineruleException, odbc::SQLException);

    /**
     * @param l a list of predicates 
     *
     * It iterates throug the list l
     * maintaining a set of intervals corresponding to the attributes
     * in l. It returns false as soon one of those intervals becomes empty. It returns
     * true if at the end of the list all intervals are still non-empty.
     */
    bool impossibleVariableSetting( const VarSet& vset, const list_AND_node* l);
  };
}

#endif
