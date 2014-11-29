#include "eval.h"
#include "ast.h"
#include "st.h"
#include <stdlib.h>

/* Voir typer.c */
extern st_t* glob_st;

double eval_factor(factor_t *const f)
{
  double res = 0;
  entry_t* e = NULL;

  switch (f->k) {
    case NUM:
      res = f->u.num->k == INT ? (double)f->u.num->u.i : f->u.num->u.d;
      break;

    case EXPR: res = eval_expr(f->u.expr); break;
      
    case IDENT:
      e = get_symbol(f->u.id, glob_st);
      res = e ? e->val : 0;
      break;
      
    case BOOL: res = f->u.bool_val; break;
    default: break;
  }
  return res;
}

double eval_term(term_t *const t)
{
  double f = eval_factor(t->fact);
  double res = f;
  termseq_t* seq = t->rest;

  while (seq) {
    switch (seq->op) {
      case TIMES: res *= eval_factor(seq->fact); break;
      case SLASH: res /= eval_factor(seq->fact); break;
      case DIV: res = (int) res / eval_factor(seq->fact); break;
      case MOD: res = (int) res % (int)eval_factor(seq->fact); break;
      case AMP: res = res == 0 ? res : eval_factor(seq->fact); break;
      default: break;
    }
    seq = seq->next;
  }

  return res;
}

double eval_simplexpr(simplexpr_t *const se)
{
  double term = eval_term(se->term);
  double res = term;
  simplexprseq_t* seq = se->rest;

  while (seq) {
    switch (seq->op) {
      case PLUS: res += eval_term(seq->term); break;
      case MINUS: res -= eval_term(seq->term); break;
      case OR: res = res == 1 ? res : eval_term(seq->term); break;
      default: break;
    }
    seq = seq->next;
  }

  return se->sign == NEG ? -res : res;
}

double eval_expr(expr_t* e)
{
  double se1 = eval_simplexpr(e->e1);
  double se2;

  if (e->e2) {
    se2 = eval_simplexpr(e->e2);
    switch (e->op) {
      case EQ: return (double) (se1 == se2);
      case NEQ: return (double) (se1 != se2);
      case LT: return (double) (se1 < se2);
      case LE: return (double) (se1 <= se2);
      case GT: return (double) (se1 > se2);
      case GE: return (double) (se1 >= se2);
      default: break;
    }
  }
  return se1;        
}
