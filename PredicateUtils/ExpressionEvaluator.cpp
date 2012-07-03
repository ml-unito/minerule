#include "ExpressionEvaluator.h"
#include <cerrno>


namespace minerule {

#define NUMPLACEHOLDER "@!x!@"


  ExpressionEvaluator::ExpressionEvaluator(list_OR_node* expr) {
    if(expr==NULL || expr->l_and==NULL)
      return;
  
    ExprToVarMapper mapper;
    expression=buildOrExpression(expr,mapper);
    varset.setSize(mapper.size());
  }

  ExpressionEvaluator::ExpressionEvaluator(Predicate& expr) {
    
  }

  ExpressionEvaluator::~ExpressionEvaluator() {
    if(expression==NULL)
      return;

    ListOR::iterator itOR;
    for(itOR=expression->begin(); itOR!=expression->end(); itOR++) {
      assert(*itOR!=NULL);
      delete *itOR;
    }

    delete expression;
  }

  bool ExpressionEvaluator::evaluate() const {
    if(expression->empty())
      return true;

    ListOR::const_iterator itOR = expression->begin();
    bool trueOrExpr=false;
  
    for(;itOR!=expression->end() && !trueOrExpr; itOR++) {
      ListAND::const_iterator itAND = (*itOR)->begin();
      bool trueAndExpr=true;
      for(;itAND!=(*itOR)->end() && trueAndExpr; itAND++) {
	trueAndExpr &= varset.getVar(*itAND);
      }

      trueOrExpr |= trueAndExpr;
    }
    return trueOrExpr;
  }

  ExpressionEvaluator::ListOR*
  ExpressionEvaluator::buildOrExpression(list_OR_node* expr, ExprToVarMapper& mapper) {
    if( expr==NULL || expr->l_and==NULL )
      return NULL;

    ListOR* result = new ListOR();

    while( expr!=NULL ) {
      result->push_back( buildAndExpression(expr->l_and, mapper) );
      expr=expr->next;
    }

    return result;
  }

  ExpressionEvaluator::ListAND*
  ExpressionEvaluator::buildAndExpression(list_AND_node* expr, ExprToVarMapper& mapper) {
    assert( expr!=NULL );

    ListAND* result = new ListAND();

    while( expr!=NULL ) {
      result->push_back(buildSPExpression(expr->sp, mapper));
      expr = expr->next;
    }

    return result;
  }

  bool
  ExpressionEvaluator::isNumber(const string& str) {
    const char* start=str.c_str();
    char* end = NULL;

    strtof(start,&end);
    return *end=='\0' && errno!=ERANGE;
  }

  ExpressionEvaluator::Var
  ExpressionEvaluator::buildSPExpression(simple_pred* sp, ExprToVarMapper& mapper) {
    assert(sp!=NULL);
    string val1 = (sp->val1!=NULL?string(sp->val1):"");
    string op = (sp->op!=NULL?string(sp->op):"");
    string val2 = (sp->val2!=NULL?string(sp->val2):"");
  
    if(isNumber(val1)) {
      val1=NUMPLACEHOLDER;
    }

    if(isNumber(val2)) {
      val2=NUMPLACEHOLDER;
    }

    string expr=val1+op+val2;
  
    ExprToVarMapper::iterator it= mapper.find(expr);
    int varNum;
  
    if(it==mapper.end()) {
      varNum = mapper.size();
      mapper[expr]=varNum;
    } else {
      varNum = it->second;
    }

    return varNum;
  }


} // namespace
