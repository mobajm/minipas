#ifndef _AST_H_
#define _AST_H_

/* Forward declarations de types, pour pouvoir utiliser
   des types non définis. */
typedef struct prog_s prog_t;

typedef struct declseq_s declseq_t;
typedef struct constseq_s constseq_t;
typedef struct typeseq_s typeseq_t;
typedef struct varseq_s varseq_t;
typedef struct decl_s decl_t;

typedef struct statseq_s statseq_t;
typedef struct assign_s assign_t;
typedef struct implyseq_s implyseq_t;
typedef struct if_s if_t;
typedef struct while_s while_t;
typedef struct stat_s stat_t;

typedef struct exprseq_s exprseq_t;
typedef struct expr_s expr_t;
typedef struct simplexpr_s simplexpr_t;
typedef struct simplexprseq_s simplexprseq_t;
typedef struct term_s term_t;
typedef struct termseq_s termseq_t;
typedef struct factor_s factor_t;

/* ========== Operators  ========== */
typedef enum {PLUS, MINUS, OR, A_NONE} addop_t;
typedef enum {POS, NEG, S_NONE} sign_t;
typedef enum {TIMES, SLASH, DIV, MOD, AMP, M_NONE} multop_t;
typedef enum {EQ, NEQ, LT, LE, GT, GE, IS, R_NONE} rel_t;
/* END Operators */

typedef struct arrty_s arrty_t;
typedef struct pterty_s pterty_t;
typedef struct type_s type_t;

typedef struct number_s number_t;

typedef struct identseq_s identseq_t;
typedef char* ident_t;

typedef struct infos_s infos_t;

/* ========== Program  ========== */
struct prog_s {
  declseq_t* decls;
  statseq_t* stats;
  ident_t ident;
};

/* ========== Declarations ========== */
struct declseq_s {
  decl_t* decl;
  declseq_t* next;
};

struct constseq_s {
  infos_t* infos;
  ident_t ident;
  expr_t* expr;
  constseq_t* next;
};

struct typeseq_s {
  infos_t* infos;
  ident_t ident;
  type_t* ty;
  typeseq_t* next;
};

struct varseq_s {
  infos_t* infos;
  identseq_t* idlist;
  type_t* ty;
  varseq_t* next;
};

struct decl_s {
  enum {CONST, TYPE, VAR} k;
  union {
    constseq_t* cseq;
    typeseq_t* tseq;
    varseq_t* vseq;
  } u;
  infos_t* infos;
};

/* ========== Statements  ========== */
struct statseq_s {
  infos_t* infos;
  stat_t* stat;
  statseq_t* next;
};

struct assign_s {
  infos_t* infos;
  ident_t desig;
  expr_t* expr;
};

struct implyseq_s {
  infos_t* infos;
  expr_t* cond;
  statseq_t* sseq;
  struct implyseq_s* next;
};

struct if_s {
  infos_t* infos;
  implyseq_t* branches;
  statseq_t* else_branch;
};

struct while_s {
  infos_t* infos;
  expr_t* cond;
  statseq_t* body;
};

struct stat_s {
  enum {ASSIGN, IF, WHILE, EXIT} k;
  union {
    assign_t* assign;
    if_t* if_n;
    while_t* while_n;
  } u;
  infos_t* infos;
};

/* ========== Expressions  ========== */
struct exprseq_s {
  expr_t* e;
  exprseq_t* next;
};

struct expr_s {
  infos_t* infos;
  simplexpr_t* e1;
  rel_t op;
  simplexpr_t* e2;
};

struct simplexpr_s {
  infos_t* infos;
  sign_t sign;
  term_t* term;
  simplexprseq_t* rest;
};

struct simplexprseq_s {
  infos_t* infos;
  addop_t op;
  term_t* term;
  simplexprseq_t* next;
};

struct term_s {
  infos_t* infos;
  factor_t* fact;
  termseq_t* rest;
};

struct termseq_s {
  infos_t* infos;
  multop_t op;
  factor_t* fact;
  termseq_t* next;
};

struct factor_s {
  enum {NUM, NIL, CHAR, EXPR, IDENT, BOOL} k;
  union {
    number_t* num;
    char* c;
    expr_t* expr;
    ident_t id;
    int bool_val;
  } u;
  infos_t* infos;
};

/* ========== Types  ========== */
typedef enum {INTEGER=0, REAL, BOOLEAN, CHARACTER, NB_SMPLTY} smplty_t;

struct arrty_s {
  type_t* ty;
  exprseq_t* exprseq;
};

struct pterty_s {
  type_t* ty;
};

struct type_s {
  enum {SMPL_TY, ARRAY_TY, POINTER_TY} k;
  union {
    smplty_t ty;
    arrty_t* arr;
    pterty_t* pter;
  } u;
};

/* ========== Numbers ========== */
struct number_s {
  enum {INT, DOUBLE} k;
  union {
    long i;
    double d;
  } u;
  infos_t* infos;
};

/* ========== Identifier, Qualident, Designator, etc.  ========== *
 *    On utilise uniquement des identifiants simples, for now     *
 * ============================================================== */
struct identseq_s {
  infos_t* infos;
  ident_t ident;
  identseq_t* next;
};

struct infos_s {
  int line;
  int col;
  type_t* ty;
};

/* ========== Prototypes des fonctions de constructions  ========== *
 *                 des différents noeuds de l'ast                   *
 * ================================================================ */
prog_t* make_prog(ident_t, declseq_t*, statseq_t*);

declseq_t* make_declseq(decl_t*, declseq_t*);

decl_t* from_constseq(constseq_t*);
constseq_t* add_to_constseq(ident_t, expr_t*, constseq_t*);

decl_t* from_typeseq(typeseq_t*);
typeseq_t* add_to_typeseq(ident_t, type_t*, typeseq_t*);

decl_t* from_varseq(varseq_t*);
varseq_t* add_to_varseq(identseq_t*, type_t*, varseq_t*);

statseq_t* make_statseq(stat_t*, statseq_t*);
stat_t* from_assign(ident_t, expr_t*);
implyseq_t* make_implyseq(expr_t*, statseq_t*, implyseq_t*);
stat_t* from_if(implyseq_t*, statseq_t*);
stat_t* from_while(expr_t*, statseq_t*);
stat_t* from_exit(void);
/* stat_t* from_return(expr_t*); */

exprseq_t* make_exprseq(expr_t*, exprseq_t*);
expr_t* make_expr(simplexpr_t*, rel_t, simplexpr_t*);
simplexpr_t* make_simplexpr(sign_t, term_t*, simplexprseq_t*);
simplexprseq_t* make_simplexprseq(addop_t, term_t*, simplexprseq_t*);
term_t* make_term(factor_t*, termseq_t*);
termseq_t* make_termseq(multop_t, factor_t*, termseq_t*);
factor_t* from_number(number_t*);
factor_t* from_character(char*);
factor_t* from_expr(expr_t*);
factor_t* from_nil(void);
factor_t* from_ident(ident_t);
factor_t* from_bool(int);

type_t* from_smplty(smplty_t);
type_t* from_arrty(type_t*, exprseq_t*);
type_t* from_pterty(type_t*);
const char* pptype(type_t*);

number_t* from_int(char*, int);
number_t* from_double(double);

identseq_t* make_identseq(ident_t, identseq_t*);
ident_t from_string(char*);

#endif /* _AST_H_ */
