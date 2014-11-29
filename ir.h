#ifndef _IR_H_
#define _IR_H_

#include "ast.h"

typedef enum {
  CO_ADD, CO_DIV, CO_SLASH, CO_MULT, CO_SUB, CO_OR, CO_AND,
  CO_MOD, CO_JMP, CO_JNE, CO_JG,CO_JGE,CO_JL,CO_JLE,CO_JF,CO_LABEL, CO_DUP,
  CO_SWAP, CO_LOAD, CO_STORE, CO_PUSH, CO_POP, CO_PRNT, CO_EXIT,NB_COPS
} co_t;

typedef struct vmval_s {
  enum {VM_CONST, VM_VAR, VM_LABEL} k;
  union {
    double _const;
    char* var;
    char* label_name;
  } u;
} vmval_t;

typedef struct pi_s {
  co_t codop;
  vmval_t *param;
} pi_t;

typedef struct pcode_s {
  pi_t* pi;
  struct pcode_s* next;
} pcode_t;

typedef struct label_s {
	pcode_t *pc;
	char* label;
	struct label_s *next;
}label_t;

vmval_t* vmval_from_number(number_t*);
vmval_t* vmval_from_ident(ident_t);
vmval_t* vmval_from_label(char*);
pi_t* make_pi(co_t, vmval_t*);
pcode_t* add_to_pcode(pi_t*, pcode_t*);
pcode_t* merge_pcodes(pcode_t*, pcode_t*);

#endif /* _IR_H_ */
