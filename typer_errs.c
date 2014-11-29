#include "typer_errs.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

/* 1 si erreur, 0 sinon */
int terr_cnt = 0;
/* Erreurs de type */
static typerr_t* type_errors = NULL;

#define SEMERR_STR_SIZE 200
static const char semerrs[NB_SEMERRS][SEMERR_STR_SIZE] = {
  "Variable '%s' déjà déclarée",
  "Variable '%s' non déclarée", 
  "Utilisation de la variable '%s' sans initialisation",
  "Types incompatibles entre '%s' et '%s'",
  "Modification de la constante '%s'"
};

void register_type_error(int line, int col, semerrs_t err_ty,
                         const char* s1, const char* s2)
{
  typerr_t* new = checked_malloc(sizeof *new);
  typerr_t* te = type_errors;

  terr_cnt++;
  new->line = line;
  new->col = col;
  new->str = checked_malloc(SEMERR_STR_SIZE * sizeof *(new->str));
  
  switch (err_ty) {
    case SEMERR_ALREADY_DECLARED:
      sprintf(new->str, semerrs[err_ty], s1);
      break;

    case SEMERR_VAR_UNDECLARED:
      sprintf(new->str, semerrs[err_ty], s1);
      break;

    case SEMERR_UNINITIALIZED_VAR:
      sprintf(new->str, semerrs[err_ty], s1);
      break;

    case SEMERR_INCOMPATIBLE_TYPES:
      sprintf(new->str, semerrs[err_ty], s1, s2);
      break;

    case SEMERR_CONST_EDIT:
      sprintf(new->str, semerrs[err_ty], s1);
      break;

    default: break;
  }
  new->next = NULL;

  if (te) {
    while (te->next != NULL)
      te = te->next;
    te->next = new;
  } else
    type_errors = new;
}

void print_type_errors(void)
{
  if (terr_cnt) {
    while (type_errors != NULL) {
      printf("%d:%d: %s\n", type_errors->line, type_errors->col, type_errors->str);
      type_errors = type_errors->next;
    }
    exit(EXIT_FAILURE);
  }
}
