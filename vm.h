#ifndef VM_H
#define VM_H


#include "ir.h"
#include "pile.h"


void initialiser_machine();

label_t* get_label(pcode_t *pc);

pcode_t* get_adCode(label_t* tete,char* label_name);

void interpreter_pseudocode_instruction(pcode_t *pc);

void vider_pile(Pile *p);




#endif
