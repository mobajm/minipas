#ifndef _LEXER_H_
#define _LEXER_H_

#include <string.h>

int line = 1, col = 1;
#define ADJ (col += strlen(yytext))
#define NEW_LINE (++line, col = 1)

#endif /* _LEXER_H_ */
