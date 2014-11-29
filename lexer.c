#include "tokens.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

/* Sera fournit par lex. */
extern int yylex();
extern int line, col;

/* Voir parser.c */
extern void parser_error(token_t*, token_t);

/* Le token actuel */
token_t tok;
/* Lire le prochain token si 1, sinon retourner l'actuel */
int adv_tok = 1;

void next_tok(void)
{
  if (adv_tok)
    tok = (token_t) yylex();
  else
    adv_tok = 1;
}

void eat_tok(token_t t)
{
  EXPECTED_TOKS(t);

  next_tok();
  if (tok != t)
    parser_error(expctd_toks, tok);
}
