#define _BSD_SOURCE 1
#include "ast.h"
#include "utils.h"
#include "st.h"
#include "typer.h"
#include "typer_errs.h"
#include "eval.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Table de symboles */
st_t* glob_st = NULL;

/* Types simples */
type_t *bool_ty = NULL, *int_ty = NULL,
  *real_ty = NULL, *char_ty = NULL;

extern int terr_cnt;

/* ========== Utilitaires sur les types ========== *
 * =============================================== */
void check_equal_ty(type_t* t1, type_t* t2, infos_t* inf)
{
  if (t1 != NULL && t2 != NULL) {
    if (t1->k == t2->k) {
      if (t1->k == SMPL_TY && t1->u.ty != t2->u.ty)
        register_type_error(inf->line, inf->col, SEMERR_INCOMPATIBLE_TYPES, pptype(t1), pptype(t2));

      if (t1->k == ARRAY_TY)
        check_equal_ty(t1->u.arr->ty, t2->u.arr->ty, inf);

      if (t1->k == POINTER_TY)
        check_equal_ty(t1->u.pter->ty, t2->u.pter->ty, inf);
    } else
      register_type_error(inf->line, inf->col, SEMERR_INCOMPATIBLE_TYPES, pptype(t1), pptype(t2));
  }
}

void check_comp_ty(type_t* t1, type_t* t2, infos_t* inf)
{
  int comp = 0;

  if (t1 != NULL && t2 != NULL) {
    if (t1->k == t2->k) {
      if (t1->k == SMPL_TY) {
        if (t1->u.ty == BOOLEAN && t2->u.ty == BOOLEAN)
          comp = 1;

        if ((t1->u.ty == INTEGER && t2->u.ty == INTEGER)
            || (t1->u.ty == INTEGER && t2->u.ty == REAL)
            || (t1->u.ty == REAL && t2->u.ty == INTEGER)
            || (t1->u.ty == REAL && t2->u.ty == REAL))
          comp = 1;
      }
    }

    if (!comp)
      register_type_error(inf->line, inf->col, SEMERR_INCOMPATIBLE_TYPES, pptype(t1), pptype(t2));
  }
}

type_t* addop_type(addop_t op)
{
  switch (op) {
    case PLUS: return real_ty;
    case MINUS: return real_ty;
    case OR: return bool_ty;
    default: break;
  }
  /* Never reached */
  return NULL;
}

type_t* multop_type(multop_t op)
{
  switch (op) {
    case TIMES: return real_ty;
    case SLASH: return real_ty;
    case DIV: return real_ty;
    case MOD: return int_ty;
    case AMP: return bool_ty;
    default: break;
  }
  /* Never reached */
  return NULL;
}

/* ========== Vérifications sémantiques ========== *
 * =============================================== */

/* ========== Instructions ========== */
void check_expr(expr_t*);
void check_statseq(statseq_t*);

void check_while(while_t* wh)
{
  check_expr(wh->cond);
  check_equal_ty(TYPE(wh->cond), bool_ty, wh->cond->infos);
  check_statseq(wh->body);
}

void check_if(if_t* if_n)
{
  implyseq_t* ims = if_n->branches;
  while (ims) {
    check_expr(ims->cond);
    check_equal_ty(TYPE(ims->cond), bool_ty, ims->cond->infos);
    check_statseq(ims->sseq);
    ims = ims->next;
  }

  if (if_n->else_branch)
    check_statseq(if_n->else_branch);
}

void check_assign(assign_t* ass)
{
  entry_t* e = NULL;

  check_expr(ass->expr);
  e = get_symbol(ass->desig, glob_st);

  if (e == NULL)
    REG_TE(ass->expr, SEMERR_VAR_UNDECLARED, ass->desig);
  else if (e->ro)
    REG_TE(ass->expr, SEMERR_CONST_EDIT, ass->desig);
  else {
    set_symbol_val(ass->desig, eval_expr(ass->expr), glob_st);
    check_comp_ty(e->ty, TYPE(ass->expr), ass->expr->infos);
  }
}

void check_stat(stat_t* stat)
{
  switch (stat->k) {
    case ASSIGN: check_assign(stat->u.assign); break;
    case IF: check_if(stat->u.if_n); break;
    case WHILE: check_while(stat->u.while_n); break;
    default: break;
  }
}

void check_statseq(statseq_t* stats)
{
  statseq_t* ss = stats;
  while (ss) {
    check_stat(ss->stat);
    ss = ss->next;
  }
}

/* ========== Expressions ========== */
void check_factor(factor_t* f)
{
  entry_t* e = NULL;

  switch (f->k) {
    case NUM: SET_TY(f, real_ty); break;
    case CHAR: SET_TY(f, char_ty); break;
    case BOOL: SET_TY(f, bool_ty); break;
    case IDENT:
      e = get_symbol(f->u.id, glob_st);
      if (e == NULL)
        REG_TE(f, SEMERR_VAR_UNDECLARED, f->u.id);
      else if (!e->is_init)
        REG_TE(f, SEMERR_UNINITIALIZED_VAR, f->u.id);
      else
        SET_TY(f, e->ty);
      break;

    default: break;
  }
}

void check_termseq(termseq_t* ts, type_t* ty)
{
  termseq_t* terms = ts;

  while (terms) {
    check_factor(terms->fact);
    check_comp_ty(ty, TYPE(terms->fact), terms->infos);
    check_comp_ty(ty, multop_type(terms->op), terms->infos);
    terms = terms->next;
  }
}

void check_term(term_t* t)
{
  check_factor(t->fact);
  if (t->rest)
    check_termseq(t->rest, TYPE(t->fact));
  SET_TY(t, TYPE(t->fact));
}

void check_smplexprseq(simplexprseq_t* ses, type_t* ty)
{
  simplexprseq_t* s = ses;

  while (s) {
    check_term(s->term);
    check_comp_ty(ty, TYPE(s->term), s->infos);
    check_comp_ty(ty, addop_type(s->op), s->infos);
    s = s->next;
  }
}

void check_smplexpr(simplexpr_t* se)
{
  check_term(se->term);
  if (se->sign != S_NONE) {
    if (TYPE(se->term) != int_ty && TYPE(se->term) != real_ty)
      REG_TE2(se->term, SEMERR_INCOMPATIBLE_TYPES, pptype(TYPE(se->term)),
             "INTEGER OR REAL");
  }
  check_smplexprseq(se->rest, TYPE(se->term));
  SET_TY(se, TYPE(se->term));
}

void check_expr(expr_t *const e)
{
  check_smplexpr(e->e1);
  if (e->e2) {
    check_smplexpr(e->e2);
    check_comp_ty(TYPE(e->e1), TYPE(e->e2), e->infos);
    SET_TY(e, bool_ty);
  } else
    SET_TY(e, TYPE(e->e1));
}

/* ========== Déclarations ========== */
void check_varseq(varseq_t *const vseq)
{
  varseq_t* vs = vseq;
  entry_t* e = NULL;
  identseq_t* ids = NULL;

  while (vs) {
    ids = vs->idlist;
    while (ids) {
      e = get_symbol(ids->ident, glob_st);
      if (e != NULL)
        REG_TE(ids, SEMERR_ALREADY_DECLARED, ids->ident);
      else
        add_symbol(make_entry(ids->ident, vs->ty, 0, 0), glob_st);

      ids = ids->next;
    }
    vs = vs->next;
  }
}

void check_typeseq(typeseq_t *const tseq)
{
  typeseq_t* ts = tseq;

  while (ts) {
    /* INSERT in types st */
    ts = ts->next;
  }
}

void check_constseq(constseq_t *const cseq)
{
  constseq_t* cs = cseq;
  entry_t *e = NULL;

  while (cs) {
    check_expr(cs->expr);
    
    e = get_symbol(cs->ident, glob_st);
    if (e != NULL)
      REG_TE(cs, SEMERR_ALREADY_DECLARED, cs->ident);
    else 
      add_symbol(make_entry(cs->ident, TYPE(cs->expr), 1, eval_expr(cs->expr)),
                 glob_st);
  
    cs = cs->next;
  }
}

void check_decl(decl_t *const decl)
{
  if (decl->k == CONST)
    check_constseq(decl->u.cseq);
  else if (decl->k == TYPE)
    /* check_typeseq(decl->u.tseq) */;
  else if (decl->k == VAR)
    check_varseq(decl->u.vseq);
}

void check_decls(declseq_t *const decls)
{
  declseq_t* ds = decls;
  while (ds) {
    check_decl(ds->decl);
    ds = ds->next;
  }
}

int check_prog(prog_t *const p)
{
  /* Initialisation */
  glob_st = make_st();
  bool_ty = from_smplty(BOOLEAN);
  int_ty = from_smplty(INTEGER);
  real_ty = from_smplty(REAL);
  char_ty = from_smplty(CHAR);

  /* Verifications */
  check_decls(p->decls);
  check_statseq(p->stats);
  print_type_errors();

  return terr_cnt;
}
