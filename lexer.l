%{
  #include "tokens.h"
%}
%option noyywrap

%%

IDENT         [A-Za-z][A-Za-z0-9]*
HEX           [0-9A-F]
INT           [0-9]+|[0-9]{HEX}*"H"
CHAR          [0-9]{HEX}*"X"

%%
"BEG"        { return BEG; }
"END"        { return END; }
"DOT"        { return DOT; }
"CONST"        { return CONST; }
"="        { return EQ; }
";"        { return SEMI_COL; }
"TYPE"        { return TYPE; }
"VAR"        { return VAR; }
":"        { return COLON; }
"ARRAY"        { return ARRAY; }
","        { return COMMA; }
"OF"        { return OF; }
"POINTER"        { return POINTER; }
"TO"        { return TO; }
":="        { return ASSIGN; }
"("        { return LPAR; }
")"        { return RPAR; }
"IF"        { return IF; }
"THEN"        { return THEN; }
"ELSIF"        { return ELSIF; }
"ELSE"        { return ELSE; }
"WHILE"        { return WHILE; }
"DO"        { return DO; }
"EXIT"        { return EXIT; }
"RETURN"        { return RETURN; }
"+"        { return PLUS; }
"-"        { return MINUS; }
"NIL"        { return NIL; }
"#"        { return NEQ; }
"<"        { return LT; }
"<="        { return LE; }
">"        { return GT; }
">="        { return GE; }
"IS"        { return IS; }
"OR"        { return OR; }
"*"        { return TIMES; }
"/"        { return SLASH; }
"DIV"        { return DIV; }
"MOD"        { return MOD; }
"&"        { return AMP; }
"["        { return LBRA; }
"]"        { return RBRA; }
"^"        { return HAT; }

%%
