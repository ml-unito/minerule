#ifndef __EXPRESSIONEVALUATOR_H__
#define __EXPRESSIONEVALUATOR_H__

#include "Parsers/ParsedMinerule.h"
#include <vector>
#include <map>

#include "VarSet.h"

/* **************************************************
   ExpressionEvaluator
   ---
   The expression evaluator allows to evaluate the result
   of the condition present in a mine rule having fixed
   the truth values of basic expressions inside the expression itself.
  
   - ExpressionEvaluator : it takes a boolean expression containing a number
              of propos. predicates. It substitutes those predicates with
	      variables. After this operations, it is possible to assign truth
	      values to those variables and evaluate the expression.
	      Note that the ground variables are implemented using VarSets (see
	      VarSet.h)
   ************************************************** */

namespace minerule {


class ExpressionEvaluator {
  typedef size_t Var;
  typedef vector<Var> ListAND;
  typedef vector<ListAND*> ListOR;

  ListOR* expression;
  VarSet varset;


  typedef map<string,int> ExprToVarMapper;
  static ListOR*
    buildOrExpression(list_OR_node* expr, ExprToVarMapper& mapper);
  static ListAND*
    buildAndExpression(list_AND_node* expr, ExprToVarMapper& mapper);
  static bool
    isNumber(const string& str);
  static Var
    buildSPExpression(simple_pred* sp, ExprToVarMapper& mapper);
  
 public:
  ExpressionEvaluator(Predicate& expr);
  
  ~ExpressionEvaluator();


  VarSet& getVariables() {
    return varset;
  }

  size_t numVariables() const {
    return varset.size();
  }

  void setVars(const VarSet& vset) {
    assert( varset.size() == vset.size() );
    varset = vset;
  }

  bool evaluate() const;

  bool evaluate(const VarSet& vset) {
    setVars(vset);
    return evaluate();
  }
};

} // namespace

#endif
