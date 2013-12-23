%{
  #include "tokens.h"
  #include "lexer.h"
  #include <stdio.h>
  #include <stdlib.h>
%}
%option noyywrap

IDENT         [A-Za-z][A-Za-z0-9]*
HEX           [0-9A-F]
INT           [0-9]+|[0-9]{HEX}*"H"
REAL          [0-9]+"."[0-9]*
CHAR          [0-9]{HEX}*"X"
TYPE          "INTEGER"|"BOOLEAN"|"CHAR"|"REAL"

%%
"BEG"        { ADJ; printf("BEG "); }
"END"        { ADJ; printf("END "); }
"DOT"        { ADJ; printf("DOT "); }
"CONST"        { ADJ; printf("CONST "); }
"="        { ADJ; printf("EQ "); }
";"        { ADJ; printf("SEMI_COL "); }
"TYPE"        { ADJ; printf("TYPE "); }
"VAR"        { ADJ; printf("VAR "); }
":"        { ADJ; printf("COLON "); }
"ARRAY"        { ADJ; printf("ARRAY "); }
","        { ADJ; printf("COMMA "); }
"OF"        { ADJ; printf("OF "); }
"POINTER"        { ADJ; printf("POINTER "); }
"TO"        { ADJ; printf("TO "); }
":="        { ADJ; printf("ASSIGN "); }
"("        { ADJ; printf("LPAR "); }
")"        { ADJ; printf("RPAR "); }
"IF"        { ADJ; printf("IF "); }
"THEN"        { ADJ; printf("THEN "); }
"ELSIF"        { ADJ; printf("ELSIF "); }
"ELSE"        { ADJ; printf("ELSE "); }
"WHILE"        { ADJ; printf("WHILE "); }
"DO"        { ADJ; printf("DO "); }
"EXIT"        { ADJ; printf("EXIT "); }
"RETURN"        { ADJ; printf("RETURN "); }
"+"        { ADJ; printf("PLUS "); }
"-"        { ADJ; printf("MINUS "); }
"NIL"        { ADJ; printf("NIL "); }
"#"        { ADJ; printf("NEQ "); }
"<"        { ADJ; printf("LT "); }
"<="        { ADJ; printf("LE "); }
">"        { ADJ; printf("GT "); }
">="        { ADJ; printf("GE "); }
"IS"        { ADJ; printf("IS "); }
"OR"        { ADJ; printf("OR "); }
"*"        { ADJ; printf("TIMES "); }
"/"        { ADJ; printf("SLASH "); }
"DIV"        { ADJ; printf("DIV "); }
"MOD"        { ADJ; printf("MOD "); }
"&"        { ADJ; printf("AMP "); }
"["        { ADJ; printf("LBRA "); }
"]"        { ADJ; printf("RBRA "); }
"^"        { ADJ; printf("HAT "); }
"TRUE"     { ADJ; printf("TRUE "); }
"FALSE"    { ADJ; printf("FALSE "); }
{TYPE}       { ADJ; printf("TYPE "); }
{IDENT}      { ADJ; printf("IDENT "); }
{INT}        { ADJ; printf("INT "); }
{REAL}       { ADJ; printf("REAL "); }
{CHAR}       { ADJ; printf("CHAR "); }
[\n]       { NEW_LINE; }
[\t ]+     { ADJ; }
.          { fprintf(stderr, "%d:%d: Lexème non reconnu: %s\n", line, col, yytext); ADJ; }

%%
