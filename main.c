#include <stdlib.h>
#include <stdio.h>

extern int yylex();

int main(void)
{
  yylex();
  return 0;
}
