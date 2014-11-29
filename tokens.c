#define _BSD_SOURCE 1                   /* strdup */
#include "tokens.h"
#include <stdio.h>
#include <string.h>

char* pptok(token_t t)
{
  const char tokens_as_strings[NB_TOKENS][15] = {
    "<BEGIN>", "<END>", "<DOT>", "<CONST>", "<EQ>", "<SEMICOL>", "<REAL>",
    "<VAR>", "<COLON>", "<ARRAY>", "<COMMA>", "<OF>", "<POINTER>", "<TO>",
    "<ASSIGN>", "<LPAR>", "<RPAR>", "<IF>", "<THEN>", "<ELSIF>", "<ELSE>",
    "<WHILE>", "<DO>", "<EXIT>", "<RETURN>", "<PLUS>", "<MINUS>", "<NIL>",
    "<NEQ>", "<LT>", "<LE>", "<GT>", "<GE>", "<IS>", "<OR>", "<TIMES>",
    "<SLASH>", "<DIV>", "<MOD>", "<AMP>", "<LBRA>", "<RBRA>", "<HAT>",
    "<IDENT>", "<TRUE>", "<T_FALSE>", "<INTEGER>", "<CHARACTER>", "<BOOLEAN>",
    "<INT_CONST>", "<REAL_CONST>", "<CHAR_CONST>", "<INT>", "<TYPE>"
  };
  
  return strdup(tokens_as_strings[(int)t]);
}
