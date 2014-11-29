#define _BSD_SOURCE 1
#include "ir.h"
#include "utils.h"
#include "ast.h"
#include <stdlib.h>
#include <string.h>

vmval_t* vmval_from_number(number_t* num)
{
  vmval_t* val = checked_malloc(sizeof *val);
  val->k = VM_CONST;
  val->u._const = (num->k == INT) ? (double) num->u.i : num->u.d;
  return val;
}

vmval_t* vmval_from_ident(ident_t id)
{
  vmval_t* val = checked_malloc(sizeof *val);
  val->k = VM_VAR;
  val->u.var = (char*)id;
  return val;
}

vmval_t* vmval_from_label(char* label_name)
{
  vmval_t* val = checked_malloc(sizeof *val);
  val->k = VM_LABEL;
  val->u.label_name = strdup(label_name);
  return val;
}

pi_t* make_pi(co_t codop, vmval_t* val)
{
  pi_t* pi = checked_malloc(sizeof *pi);
  pi->codop = codop;
  pi->param = val;
  return pi;
}

pcode_t* add_to_pcode(pi_t* pi, pcode_t* pcode)
{
  pcode_t* pcode_buck = checked_malloc(sizeof *pcode_buck);
  pcode_t* pcode_start = pcode;

  pcode_buck->pi = pi;
  pcode_buck->next = NULL;

  LL_INSERT_END(pcode, pcode_buck, pcode_start);
}

pcode_t* merge_pcodes(pcode_t* pc1, pcode_t* pc2)
{
  pcode_t* pc1_start = pc1;
  
  if (pc1 == NULL)
    return pc2;
  if (pc2 == NULL)
    return pc1;

  while (pc1->next != NULL)
    pc1 = pc1->next;
  pc1->next = pc2;

  return pc1_start;
}
