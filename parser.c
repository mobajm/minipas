#define _BSD_SOURCE 1
#include "utils.h"
#include "tokens.h"
#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Voir lexer.c */
extern token_t tok;
extern int adv_tok;
/* Valeur sémantique associée au token actuel */
extern tokval_t tokval;

/* 1 si une erreur se produit pendant le parsing, 0 sinon */
static int error = 0;
/* Voir lex.l */
extern int line, col, yyleng;

/* ========== Utils  ========== */
static sign_t sign_of_tok(token_t t)
{
  switch (t) {
    case T_PLUS: return POS;
    case T_MINUS: return NEG;
    default: return S_NONE;
  }
}

static addop_t addop_of_tok(token_t t)
{
  switch (t) {
    case T_PLUS: return PLUS;
    case T_MINUS: return MINUS;
    case T_OR: return OR;
    default: return A_NONE;      
  }
}

static rel_t rel_of_tok(token_t t)
{
  switch (t) {
    case T_EQ: return EQ;
    case T_NEQ: return NEQ;
    case T_LT: return LT;
    case T_GT: return GT;
    case T_LE: return LE;
    case T_GE: return GE;
    case T_IS: return IS;
    default: return R_NONE;
  }
}

static multop_t multop_of_tok(token_t t)
{
  switch (t) {
    case T_TIMES: return TIMES;
    case T_SLASH: return SLASH;
    case T_DIV: return DIV;
    case T_MOD: return MOD;
    case T_AMP: return AMP;
    default: return M_NONE;
  }
}

static char* string_of_toks(token_t* t)
{
#define STR_SIZE 100
  char *s = checked_malloc(STR_SIZE);
  
  while (*(t+1) != NB_TOKENS) {
    strcat(s, pptok(*t++));
    strcat(s, " OR ");
  }
  strcat(s, pptok(*t));
  
  return s;
}

/* ========== Gestion d'erreurs  ========== */
void parser_error(token_t* exp, token_t fnd)
{
  char *toks_exp = string_of_toks(exp);
  error = 1;
  
  printf("%d:%d: Expected %s, found %s\n",
         line, col - yyleng, toks_exp, pptok(fnd));

  free(toks_exp);
  exit(EXIT_FAILURE);
  
  /* FIXME: Implementer une récupération en cas d'erreur,
            pour pousser le parsing le plus loin possible
            et détecter le maximum d'erreurs. */
}

/* ========== Identifiants ========== */
ident_t p_ident(void)
{
  EXPECTED_TOKS(T_IDENT);

  if (tok == T_IDENT)
    return (ident_t) strdup(tokval.ident);
  else
    parser_error(expctd_toks, tok);
  
  return NULL;
}

identseq_t* p_identseq(void)
{
  ident_t idf;
  identseq_t* idseq = NULL;

  idf = p_ident();
  do {
    idseq = make_identseq(idf, idseq);
    next_tok();
    if (tok == T_COMMA) {
      next_tok();
      idf = p_ident();
    } else {
      adv_tok = 0;
      break;
    }
  } while (1);

  return idseq;
}

/* ========== Types ========== */
smplty_t p_smpltype(void)
{
  EXPECTED_TOKS(T_INTEGER_TY, T_REAL_TY, T_CHARACTER_TY, T_BOOLEAN_TY);
  switch (tok) {
    case T_INTEGER_TY: return INTEGER;
    case T_REAL_TY: return REAL;
    case T_CHARACTER_TY: return CHARACTER;
    case T_BOOLEAN_TY: return BOOLEAN;
    default: parser_error(expctd_toks, tok); /* Jamais executé */ return 0;
  }
}

exprseq_t* p_exprseq(void);
type_t* p_type(void)
{
  exprseq_t* eseq = NULL;
  type_t* ty = NULL;

  if (tok == T_POINTER) {
    eat_tok(T_TO);
    next_tok();
    ty = p_type();
    return from_pterty(ty);
  } else if (tok == T_ARRAY) {
    next_tok();
    if (tok == T_OF) {
      next_tok();
      ty = p_type();
      return from_arrty(ty, NULL);
    } else {
      eseq = p_exprseq();
      eat_tok(T_OF);
      next_tok();
      ty = p_type();
      return from_arrty(ty, eseq);
    }
  } else {
    return from_smplty(p_smpltype());
  }
}

/* ========== Instructions  ========== */
expr_t* p_expr(void);
statseq_t* p_statseq(void);
stat_t* p_stat(void)
{
  expr_t* e = NULL;
  statseq_t* sseq = NULL, *else_sseq = NULL, *elsifs = NULL;
  implyseq_t* ims = NULL;
  ident_t id = NULL;

  if (tok == T_WHILE) {
    next_tok();
    e = p_expr();
    eat_tok(T_DO);
    next_tok();
    sseq = p_statseq();
    eat_tok(T_END);
    return from_while(e, sseq);
  } else if (tok == T_EXIT) {
    return from_exit();
  } else if (tok == T_IF) {
    next_tok();
    e = p_expr();
    eat_tok(T_THEN);
    next_tok();
    sseq = p_statseq();
    ims = make_implyseq(e, sseq, ims);
    /* Else ifs */
    while (tok == T_ELSIF) {
      adv_tok = 1;
      next_tok();
      e = p_expr();
      eat_tok(T_THEN);
      next_tok();
      elsifs = p_statseq();
      ims = make_implyseq(e, elsifs, ims);
      adv_tok = 1;
      next_tok();
    }
    /* Else, optionnel */
    if (tok == T_ELSE) {
      adv_tok = 1;
      next_tok();
      else_sseq = p_statseq();
      adv_tok = 0;
    }
    adv_tok = 0;
    eat_tok(T_END);
    return from_if(ims, else_sseq);
  } else {
    id = p_ident();
    eat_tok(T_ASSIGN);
    next_tok();
    e = p_expr();
    return from_assign(id, e);
  }
}

statseq_t* p_statseq(void)
{
  stat_t* s = p_stat();
  statseq_t* sseq = make_statseq(s, NULL);

  next_tok();
  while (tok == T_SEMICOL) {
    next_tok();
    s = p_stat();
    sseq = make_statseq(s, sseq);
    next_tok();
  }
  adv_tok = 0;

  return sseq;
}

/* ========== Expressions ========== */
factor_t* p_factor(void)
{
#define TRUE 1
#define FALSE 0
  
  EXPECTED_TOKS(T_INT, T_REAL, T_CHAR, T_NIL, T_IDENT, T_TRUE, T_FALSE, T_LPAR);

  factor_t* f = NULL;
  expr_t* e = NULL;

  switch (tok) {
    case T_LPAR:
      next_tok();
      e = p_expr();
      eat_tok(T_RPAR);
      f = from_expr(e);
      break;
      
    case T_INT:
      f = from_number(from_int(tokval.i, 10));
      break;

    case T_HINT:
      f = from_number(from_int(tokval.i, 16));
      break;
      
    case T_REAL:
      f = from_number(from_double(tokval.d));
      break;
      
    case T_CHAR:
      f = from_character(tokval.c);
      break;
      
    case T_NIL:
      f = from_nil();
      break;

    case T_IDENT:
      f = from_ident(from_string(tokval.ident));
      break;

    case T_TRUE:
      f = from_bool(TRUE);
      break;

    case T_FALSE:
      f = from_bool(FALSE);
      break;

    default:
      parser_error(expctd_toks, tok);
  }
  next_tok();
  
  return f;
}

termseq_t* p_termseq(void)
{
  multop_t mop;
  factor_t* f = NULL;
  termseq_t* ts = NULL;

  while ((mop = multop_of_tok(tok)) != M_NONE) {
    next_tok();
    f = p_factor();
    ts = make_termseq(mop, f, ts);
  }
  
  adv_tok = 0;
  return ts;
}

term_t* p_term(void)
{
  factor_t* f = NULL;
  
  f = p_factor();
  return make_term(f, p_termseq());
}

simplexprseq_t* p_simplexprseq(void)
{
  simplexprseq_t* ses = NULL;
  addop_t addop;
  term_t* t = NULL;

  while ((addop = addop_of_tok(tok)) != A_NONE) {
    /* Le token doit avancer */
    adv_tok = 1;
    next_tok();
    t = p_term();
    ses = make_simplexprseq(addop, t, ses);
    next_tok();
  }
  
  adv_tok = 0;
  return ses;
}

simplexpr_t* p_simplexpr(void)
{
  sign_t s;
  term_t* t = NULL;

  s = sign_of_tok(tok);
  if (s != S_NONE) {
    next_tok();
  }
  t = p_term();

  return make_simplexpr(s, t, p_simplexprseq());
}

expr_t* p_expr(void)
{
  simplexpr_t *se1 = NULL, *se2 = NULL;
  rel_t r;
  
  se1 = p_simplexpr();
  
  next_tok();
  r = rel_of_tok(tok);
  if (r != R_NONE) {
    next_tok();
    se2 = p_simplexpr();
  } else {
    adv_tok = 0;
  }

  return make_expr(se1, r, se2);
}

exprseq_t* p_exprseq(void)
{
  exprseq_t* eseq = NULL;
  expr_t* e = NULL;

  e = p_expr();
  eseq = make_exprseq(e, eseq);

  next_tok();
  while (tok == T_COMMA) {
    adv_tok = 1;
    next_tok();
    e = p_expr();
    eseq = make_exprseq(e, eseq);
    next_tok();
  }
  adv_tok = 0;

  return eseq;

}

/* ========== Declarations ========== */
decl_t* p_vardecls(void)
{
  varseq_t* vs = NULL;
  identseq_t* idseq = NULL;
  type_t* t = NULL;

  next_tok();
  while (tok == T_IDENT) {
    idseq = p_identseq();
    eat_tok(T_COLON);
    next_tok();
    t = p_type();
    eat_tok(T_SEMICOL);
    vs = add_to_varseq(idseq, t, vs);
    next_tok();
  }
  adv_tok = 0;
  
  return from_varseq(vs);
}

decl_t* p_typedecls(void)
{
  typeseq_t* ts = NULL;
  ident_t idf;
  type_t* t = NULL;

  next_tok();
  while (tok == T_IDENT) {
    idf = p_ident();
    eat_tok(T_EQ);
    next_tok();
    t = p_type();
    eat_tok(T_SEMICOL);

    ts = add_to_typeseq(idf, t, ts);
    next_tok();
  }
  adv_tok = 0;

  return from_typeseq(ts);
}

decl_t* p_constdecls(void)
{
  constseq_t* cs = NULL;
  ident_t idf;
  expr_t* e = NULL;

  next_tok();
  while (tok == T_IDENT) {
    idf = p_ident();
    eat_tok(T_EQ);
    next_tok();
    e = p_expr();
    eat_tok(T_SEMICOL);

    cs = add_to_constseq(idf, e, cs);
    next_tok();
  }
  adv_tok = 0;

  return from_constseq(cs);
}

declseq_t* p_declseq(void)
{
  declseq_t* ds = NULL;

  if (tok != T_CONST && tok != T_TYPE && tok != T_VAR)
    adv_tok = 0;
  
  while (tok == T_CONST || tok == T_TYPE || tok == T_VAR) {
    if (tok == T_CONST)
      ds = make_declseq(p_constdecls(), ds);
    else if (tok == T_TYPE)
      ds = make_declseq(p_typedecls(), ds);
    else if (tok == T_VAR)
      ds = make_declseq(p_vardecls(), ds);

    next_tok();
  }
  
  return ds;
}

prog_t* p_prog(void)
{
  declseq_t* ds = NULL;
  statseq_t* ss = NULL;
  ident_t prog_name;

  next_tok();
  ds = p_declseq();

  if (tok == T_BEG) {
    adv_tok = 1;
    next_tok();
    ss = p_statseq();
  } else {
    adv_tok = 0;
  }
  eat_tok(T_END);

  next_tok();
  prog_name = p_ident();

  eat_tok(T_DOT);

  return make_prog(prog_name, ds, ss);
}
