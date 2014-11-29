%{
  #include "tokens.h"
  #include "lexer.h"
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  /* On garde une trace de la ligne et colonne en lexant,
     pour avoir des messages d'erreur précis. */
  int line = 1, col = 1;
  
#define ADJ (col += yyleng)
#define NEW_LINE (++line, col = 1)
%}
%option noyywrap

IDENT         [A-Za-z][A-Za-z0-9]*
HEX           [0-9A-F]
INT           [0-9]+
HINT          {HEX}*"H"
REAL          [0-9]+"."[0-9]*
CHAR          [0-9]{HEX}*"X"
TYPE          "INTEGER"|"BOOLEAN"|"CHAR"|"REAL"

%%
"BEGIN"        { ADJ; return T_BEG; }
"END"        { ADJ; return T_END; }
"."        { ADJ; return T_DOT; }
"CONST"        { ADJ; return T_CONST; }
"="        { ADJ; return T_EQ; }
";"        { ADJ; return T_SEMICOL; }
"TYPE"        { ADJ; return T_TYPE; }
"VAR"        { ADJ; return T_VAR; }
":"        { ADJ; return T_COLON; }
"ARRAY"        { ADJ; return T_ARRAY; }
","        { ADJ; return T_COMMA; }
"OF"        { ADJ; return T_OF; }
"POINTER"        { ADJ; return T_POINTER; }
"TO"        { ADJ; return T_TO; }
":="        { ADJ; return T_ASSIGN; }
"("        { ADJ; return T_LPAR; }
")"        { ADJ; return T_RPAR; }
"IF"        { ADJ; return T_IF; }
"THEN"        { ADJ; return T_THEN; }
"ELSIF"        { ADJ; return T_ELSIF; }
"ELSE"        { ADJ; return T_ELSE; }
"WHILE"        { ADJ; return T_WHILE; }
"DO"        { ADJ; return T_DO; }
"EXIT"        { ADJ; return T_EXIT; }
"RETURN"        { ADJ; return T_RETURN; }
"+"        { ADJ; return T_PLUS; }
"-"        { ADJ; return T_MINUS; }
"NIL"        { ADJ; return T_NIL; }
"#"        { ADJ; return T_NEQ; }
"<"        { ADJ; return T_LT; }
"<="        { ADJ; return T_LE; }
">"        { ADJ; return T_GT; }
">="        { ADJ; return T_GE; }
"IS"        { ADJ; return T_IS; }
"OR"        { ADJ; return T_OR; }
"*"        { ADJ; return T_TIMES; }
"/"        { ADJ; return T_SLASH; }
"DIV"        { ADJ; return T_DIV; }
"MOD"        { ADJ; return T_MOD; }
"&"        { ADJ; return T_AMP; }
"["        { ADJ; return T_LBRA; }
"]"        { ADJ; return T_RBRA; }
"^"        { ADJ; return T_HAT; }
"TRUE"     { ADJ; return T_TRUE; }
"FALSE"    { ADJ; return T_FALSE; }
"INTEGER"  { ADJ; return T_INTEGER_TY; }
"CHARACTER"  { ADJ; return T_CHARACTER_TY; }
"REAL"     { ADJ; return T_REAL_TY; }
"BOOLEAN"    { ADJ; return T_BOOLEAN_TY; }
{IDENT}      { ADJ; tokval.ident = strdup(yytext); return T_IDENT; }
{INT}        { ADJ; tokval.i = strdup(yytext); return T_INT; }
{HINT}        { ADJ; yytext[yyleng - 2] = '\0';
                     tokval.i = strdup(yytext); return T_HINT; }
{REAL}       { ADJ; tokval.d = atof(yytext); return T_REAL; }
{CHAR}       { ADJ; tokval.c = strdup(yytext); return T_CHAR; }
[\n]       { NEW_LINE; }
[\t ]+     { ADJ; }
.          { fprintf(stderr, "%d:%d: Lexème non reconnu: %s\n", line, col, yytext); ADJ; }

%%
