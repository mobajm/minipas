#ifndef _TYPER_ERRS_H_
#define _TYPER_ERRS_H_

/* Erreurs sémantiques */
typedef enum {
  SEMERR_ALREADY_DECLARED = 0,
  SEMERR_VAR_UNDECLARED,
  SEMERR_UNINITIALIZED_VAR,
  SEMERR_INCOMPATIBLE_TYPES,
  SEMERR_CONST_EDIT,
  NB_SEMERRS
} semerrs_t;

/* Liste d'erreurs semantiques */
typedef struct typerr_s {
  int line, col;
  char* str;
  struct typerr_s* next;
} typerr_t;

/* Register type error */
#define REG_TE(el, errty, s)                                            \
  register_type_error((el)->infos->line, (el)->infos->col, (errty), (s), NULL)

#define REG_TE2(el, errty, s, s2)                                       \
  register_type_error((el)->infos->line, (el)->infos->col, (errty), (s), (s2))

void register_type_error(int, int, semerrs_t, const char*, const char*);
void print_type_errors(void);
   
#endif /* _TYPER_ERRS_H_ */
