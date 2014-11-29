#ifndef _ST_H_
#define _ST_H_

#include "ast.h"

typedef struct {
  char *nom;
  type_t *ty;                           /* type de variable */
  infos_t* infos;
  double val;
  int is_init;                          /* Est initialisée */
  int ro;                               /* Read Only */
} entry_t;

/* typedef struct { */
/*   int key; */
/*   entry_t* infos; */
/* } bucket_t; */

typedef struct buckets_s {
  entry_t* data;
  struct buckets_s* next;
} buckets_t;

typedef struct {
  buckets_t** data;
  int size;
} st_t;  

/* ========== Prototypes des fonctions de constructions  ========== *
 *                 de la table des symboles                          *
 * ================================================================= */
st_t* make_st(void);
entry_t* make_entry(char*, type_t*, int, double);
void add_symbol(entry_t*, st_t*);
entry_t* get_symbol(char*, st_t*);
void set_symbol_val(char*, double, st_t*);

#endif 
