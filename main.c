#include <stdlib.h>
#include <stdio.h>
#include "ast.h"
#include "lexer.h"
#include "tokens.h"
#include "parser.h"
#include "typer_errs.h"
#include "typer.h"
#include "ir.h"
#include "gen_pcode.h"
#include "vm.h"


int main(void)
{
  int sem_errs;
  prog_t* p = p_prog();
  pcode_t* pcode = NULL;

  if ((sem_errs = check_prog(p)) != 0) {
    printf("Le nombre d'erreurs sémantiques est %d\n", sem_errs);
    print_type_errors();
    exit(EXIT_FAILURE);
  }

  pcode = gen_prog(p);
  /* initialiser_machine(); */
  /* interpreter_pseudocode_instruction(pcode); */

  return 0;
}
