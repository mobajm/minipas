#ifndef _TOKENS_H_
#define _TOKENS_H_

typedef enum {
  T_BEG = 0, T_END, T_DOT, T_CONST, T_EQ, T_SEMICOL, T_REAL_TY,
  T_VAR, T_COLON, T_ARRAY, T_COMMA, T_OF, T_POINTER, T_TO,
  T_ASSIGN, T_LPAR, T_RPAR, T_IF, T_THEN, T_ELSIF, T_ELSE,
  T_WHILE, T_DO, T_EXIT, T_RETURN, T_PLUS, T_MINUS, T_NIL,
  T_NEQ, T_LT, T_LE, T_GT, T_GE, T_IS, T_OR, T_TIMES, T_SLASH,
  T_DIV, T_MOD, T_AMP, T_LBRA, T_RBRA, T_HAT, T_IDENT, T_TRUE,
  T_FALSE, T_INTEGER_TY, T_CHARACTER_TY, T_BOOLEAN_TY, T_INT,
  T_REAL, T_CHAR, T_TYPE, T_HINT, NB_TOKENS
} token_t;

/* Pretty Print token. */
char* pptok(token_t);

#define EXPECTED_TOKS(...) token_t expctd_toks[] = {__VA_ARGS__, NB_TOKENS}

#endif /* _TOKENS_H_ */
