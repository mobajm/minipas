#ifndef _TYPER_H_
#define _TYPER_H_

#include "ast.h"

/* Macros utilitaires pour la gestion des types */
#define TYPE(el) (el)->infos->ty
#define SET_TY(el, ty) TYPE((el)) = (ty)

int check_prog(prog_t*);

#endif /* _TYPER_H_ */
