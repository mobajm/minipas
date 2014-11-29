#define _BSD_SOURCE 1
#include "ast.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* See lex.l */
extern int line, col;

#define BUILD_INFOS(node, li, co)               \
  SET_LINE_COL((node), (li), (co))
/* ALLOC_INF_NODE((node));                       \ */


#define ALLOC_INF_NODE(node)                                    \
  (node)->infos = checked_malloc(sizeof *((node)->infos))       
      
#define SET_LINE_COL(node, li, co)              \
  (node)->infos->line = (li);                   \
  (node)->infos->col = (co)                    

#define LINE(node) (node)->infos->line
#define COL(node) (node)->infos->col

/* ========== Program  ========== */
prog_t* make_prog(ident_t name, declseq_t* dseq, statseq_t* sseq)
{
  prog_t* p = checked_malloc(sizeof *p);
  p->decls = dseq;
  p->stats = sseq;
  p->ident = name;
  return p;
}

/* ========== Declarations ========== */
declseq_t* make_declseq(decl_t* d, declseq_t* ds)
{
  declseq_t* dseq = ds;
  declseq_t* dcl = checked_malloc(sizeof *dseq);
  
  dcl->decl = d;
  dcl->next = NULL;

  LL_INSERT_END(ds, dcl, dseq);
}

decl_t* from_constseq(constseq_t* cs)
{
  decl_t* decl = checked_malloc(sizeof *decl);
  
  decl->k = CONST;  
  decl->u.cseq = cs;

  if (cs && cs->infos) {
    decl->infos = checked_malloc(sizeof *(decl->infos));
    BUILD_INFOS(decl, LINE(cs), COL(cs));
  }

  return decl;
}

constseq_t* add_to_constseq(ident_t id, expr_t* e, constseq_t* cs)
{
  constseq_t* cs_start = cs;
  constseq_t* new = checked_malloc(sizeof *new);
  
  new->ident = id;
  new->expr = e;
  new->next = NULL;

  if (e && cs) {
    cs->infos = checked_malloc(sizeof *(cs->infos));
    BUILD_INFOS(cs, LINE(e), COL(e));
  }

  LL_INSERT_END(cs, new, cs_start);
}

decl_t* from_typeseq(typeseq_t* ts)
{
  decl_t* decl = checked_malloc(sizeof *decl);
  
  decl->k = TYPE;  
  decl->u.tseq = ts;

  if (ts) {
    decl->infos = checked_malloc(sizeof *(decl->infos));
    BUILD_INFOS(decl, LINE(ts), COL(ts));
  }

  return decl;
}

typeseq_t* add_to_typeseq(ident_t id, type_t* t, typeseq_t* ts)
{
  typeseq_t* ts_start = ts;
  typeseq_t* new = checked_malloc(sizeof *new);
  
  new->ident = id;
  new->ty = t;
  new->next = NULL;

  ts->infos = checked_malloc(sizeof *(ts->infos));
  BUILD_INFOS(ts, line, col);

  LL_INSERT_END(ts, new, ts_start);
}

decl_t* from_varseq(varseq_t* vs)
{
  decl_t* decl = checked_malloc(sizeof *decl);
  
  decl->k = VAR;  
  decl->u.vseq = vs;

  if (vs && vs->infos) {
    decl->infos = checked_malloc(sizeof *(decl->infos));
    BUILD_INFOS(decl, LINE(vs), COL(vs));
  }

  return decl;
}

varseq_t* add_to_varseq(identseq_t* idseq, type_t* t, varseq_t* vs)
{
  varseq_t* new = checked_malloc(sizeof *new);
  varseq_t* vs_start = vs;

  new->idlist = idseq;
  new->ty = t;
  new->next = NULL;

  if (idseq && vs) {
    vs->infos = checked_malloc(sizeof *(vs->infos));
    BUILD_INFOS(vs, LINE(idseq), COL(idseq));
  }

  LL_INSERT_END(vs, new, vs_start);
}

/* ========== Statements  ========== */
statseq_t* make_statseq(stat_t* s, statseq_t* ss)
{
  statseq_t* sseq = checked_malloc(sizeof *sseq);
  statseq_t* ss_start = ss;

  sseq->stat = s;
  sseq->next = NULL;

  LL_INSERT_END(ss, sseq, ss_start);
}

stat_t* from_assign(ident_t id, expr_t* e)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = ASSIGN;

  s->u.assign = checked_malloc(sizeof *(s->u.assign));
  s->u.assign->desig = id;
  s->u.assign->expr = e;

  if (e) {
    s->infos = checked_malloc(sizeof *(s->infos));
    BUILD_INFOS(s, LINE(e), COL(e));
  }

  return s;
}

implyseq_t* make_implyseq(expr_t* e, statseq_t* sseq, implyseq_t* ims)
{
  implyseq_t* new = checked_malloc(sizeof *new);
  implyseq_t* ims_start = ims;

  new->cond = e;
  new->sseq = sseq;
  new->next = NULL;

  if (e && ims) {
    ims->infos = checked_malloc(sizeof *(ims->infos));
    BUILD_INFOS(ims, LINE(e), COL(e));
  }

  LL_INSERT_END(ims, new, ims_start);
}

stat_t* from_if(implyseq_t* brches, statseq_t* el)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = IF;

  s->u.if_n = checked_malloc(sizeof *(s->u.if_n));
  s->u.if_n->branches = brches;
  s->u.if_n->else_branch = el;

  if (brches->cond) {
    s->infos = checked_malloc(sizeof *(s->infos));
    BUILD_INFOS(s, LINE(brches->cond), COL(brches->cond));
  }

  return s;
}

stat_t* from_while(expr_t* pred, statseq_t* b)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = WHILE;

  s->u.while_n = checked_malloc(sizeof *(s->u.while_n));
  s->u.while_n->cond = pred;
  s->u.while_n->body = b;

  if (pred) {
    s->infos = checked_malloc(sizeof *(s->infos));
    BUILD_INFOS(s, LINE(pred), COL(pred));
  }

  return s;
}

stat_t* from_exit(void)
{
  stat_t* s = checked_malloc(sizeof *s);
  s->k = EXIT;
  return s;
}

/* stat_t* from_return(expr_t* e) */
/* { */
/*   stat_t* s = checked_malloc(sizeof *s); */
/*   s->k = RETURN; */
/*   s->u.ret = e; */
/*   return s; */
/* } */

/* ========== Expressions  ========== */
exprseq_t* make_exprseq(expr_t* e, exprseq_t* es)
{
  exprseq_t* eseq = checked_malloc(sizeof *eseq);
  exprseq_t* es_start = es;

  eseq->e = e;
  eseq->next = NULL;

  LL_INSERT_END(es, eseq, es_start);
}

expr_t* make_expr(simplexpr_t* e1, rel_t op, simplexpr_t* e2)
{
  expr_t* e = checked_malloc(sizeof *e);
  e->e1 = e1;
  e->op = op;
  e->e2 = e2;

  if (e1) {
    e->infos = checked_malloc(sizeof *(e->infos));
    BUILD_INFOS(e, LINE(e1), COL(e1));
  }
  return e;
}

simplexpr_t* make_simplexpr(sign_t s, term_t* t, simplexprseq_t* sexpseq)
{
  simplexpr_t* se = checked_malloc(sizeof *se);
  se->sign = s;
  se->term = t;
  se->rest = sexpseq;

  if (t) {
    se->infos = checked_malloc(sizeof *(se->infos));
    BUILD_INFOS(se, LINE(t), COL(t));
  }
  return se;
}

simplexprseq_t* make_simplexprseq(addop_t op, term_t* t, simplexprseq_t* sexps)
{
  simplexprseq_t* ses = checked_malloc(sizeof *ses);
  simplexprseq_t* ses_start = sexps;

  ses->op = op;
  ses->term = t;
  ses->next = NULL;

  if (t) {
    ses->infos = checked_malloc(sizeof *(ses->infos));
    BUILD_INFOS(ses, LINE(t), COL(t));
  }
  LL_INSERT_END(sexps, ses, ses_start);
}

term_t* make_term(factor_t* f, termseq_t* ts)
{
  term_t* t = checked_malloc(sizeof *t);
  t->fact = f;
  t->rest = ts;

  if (f) {
    t->infos = checked_malloc(sizeof *(t->infos));
    BUILD_INFOS(t, LINE(f), COL(f));
  }
  return t;
}

termseq_t* make_termseq(multop_t op, factor_t* f, termseq_t* ts)
{
  termseq_t* tseq = ts;
  termseq_t* t = checked_malloc(sizeof *t);
  
  t->op = op;
  t->fact = f;
  t->next = NULL;

  if (f) {
    t->infos = checked_malloc(sizeof *(t->infos));
    BUILD_INFOS(t, LINE(f), COL(f));
  }

  LL_INSERT_END(ts, t, tseq);
}

factor_t* from_number(number_t* n)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = NUM;
  f->u.num = n;

  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

factor_t* from_character(char* c)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = CHAR;
  f->u.c = c;
  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

factor_t* from_expr(expr_t* e)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = EXPR;
  f->u.expr = e;
  
  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

factor_t* from_nil(void)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = NIL;

  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

factor_t* from_ident(ident_t id)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = IDENT;
  f->u.id = id;

  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

factor_t* from_bool(int b)
{
  factor_t* f = checked_malloc(sizeof *f);
  f->k = BOOL;
  f->u.bool_val = b;

  f->infos = checked_malloc(sizeof *(f->infos));
  BUILD_INFOS(f, line, col);
  return f;
}

/* ========== Types  ========== */
type_t* from_smplty(smplty_t ty)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = SMPL_TY;
  t->u.ty = ty;
  return t;
}

type_t* from_arrty(type_t* ty, exprseq_t* es)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = ARRAY_TY;

  t->u.arr = checked_malloc(sizeof *(t->u.arr));
  t->u.arr->ty = ty;
  t->u.arr->exprseq = es;
  return t;
}

type_t* from_pterty(type_t* ty)
{
  type_t* t = checked_malloc(sizeof *t);
  t->k = POINTER_TY;

  t->u.pter = checked_malloc(sizeof *(t->u.pter));
  t->u.pter->ty = ty;
  return t;
}

const char* pptype(type_t* ty)
{
  char smpltypes_str[NB_SMPLTY][15] = {
    "INTEGER", "REAL", "BOOLEAN", "CHARACTER"
  };

  if (ty->k == SMPL_TY)
    return strdup(smpltypes_str[ty->u.ty]);

  return "";
}

/* ========== Numbers ========== */
number_t* from_int(char* i, int base)
{
  number_t* n = checked_malloc(sizeof *n);
  n->k = INT;
  n->u.i = strtol(i, NULL, base);

  n->infos = checked_malloc(sizeof *(n->infos));
  BUILD_INFOS(n, line, col);
  return n;
}

number_t* from_double(double d)
{
  number_t* n = checked_malloc(sizeof *n);
  n->k = DOUBLE;
  n->u.d = d;
  n->infos = checked_malloc(sizeof *(n->infos));
  BUILD_INFOS(n, line, col);
  return n;
}

/* ========== Identifier, Qualident, Designator, etc.  ========== */
identseq_t* make_identseq(ident_t id, identseq_t* idseq)
{
  identseq_t* is = checked_malloc(sizeof *is);
  identseq_t* idseq_start = idseq;
  
  is->ident = id;
  is->next = NULL;

  is->infos = checked_malloc(sizeof *(is->infos));
  BUILD_INFOS(is, line, col);
  LL_INSERT_END(idseq, is, idseq_start);
}

ident_t from_string(char* s)
{
  return (ident_t) s;
}
