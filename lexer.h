#ifndef _LEXER_H_
#define _LEXER_H_

#include "tokens.h"

typedef union {
  char* i;
  char* c;
  double d;
  char* ident;
} tokval_t;

/* Valeur associée à certains tokens */
tokval_t tokval;

/* Lit le prochain token en entrée */
void next_tok(void);
/* Verifie qu'un token est bien present, et le depasse */
void eat_tok(token_t);

#endif /* _LEXER_H_ */
