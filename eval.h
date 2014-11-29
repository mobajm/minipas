#ifndef _EVAL_H_
#define _EVAL_H_

#include "ast.h"

double eval_factor(factor_t*);
double eval_term(term_t*);
double eval_simplexpr(simplexpr_t*);
double eval_expr(expr_t*);

#endif /* _EVAL_H_ */
