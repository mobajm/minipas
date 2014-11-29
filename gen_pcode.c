#define _BSD_SOURCE 1
#include "gen_pcode.h"
#include "ir.h"
#include "ast.h"
#include "eval.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Compteur de labels */
static int lab_cnt = 0;
/* Nom du prochain label à utiliser */
static char label[20] = {0};
#define GEN_NEW_LABEL(str) sprintf(label, "%s%d", (str), lab_cnt++)

/* Helpers, factorisent des éléments qui se répètent */
#define ADD_PI(pcode, pi) (pcode) = add_to_pcode((pi), (pcode))
#define CONST_OF_EXPR(e) vmval_from_number(from_double(eval_expr((e))))
#define LABEL_VAL vmval_from_label(label)

pi_t* gen_exit(void)
{
  return make_pi(CO_EXIT, NULL);
}

pcode_t* gen_while(while_t* while_n)
{
  pcode_t* pcode = NULL;
  char *loop_lab = NULL;

  /* Label de début de boucle */
  GEN_NEW_LABEL("loop");
  loop_lab = strdup(label);
  ADD_PI(pcode, make_pi(CO_LABEL, vmval_from_label(loop_lab)));

  /* Condition d'arret */
  ADD_PI(pcode, make_pi(CO_PUSH, CONST_OF_EXPR(while_n->cond)));
  GEN_NEW_LABEL("endloop");
  ADD_PI(pcode, make_pi(CO_JF, LABEL_VAL));

  /* Corps de boucle */
  pcode = merge_pcodes(pcode, gen_statseq(while_n->body));

  /* Retour en début de boucle */
  ADD_PI(pcode, make_pi(CO_JMP, vmval_from_label(loop_lab)));
  ADD_PI(pcode, make_pi(CO_LABEL, LABEL_VAL));

  free(loop_lab);
  return pcode;
}

pcode_t* gen_if(if_t* if_node)
{
  pcode_t *pcode = NULL;
  /* Else ifs */
  implyseq_t* elsifs = if_node->branches->next;

  ADD_PI(pcode, make_pi(CO_PUSH, CONST_OF_EXPR(if_node->branches->cond)));
  
  if (elsifs) {
    /* Generation de label else if et du test correspondant */
    GEN_NEW_LABEL("elseif");
    ADD_PI(pcode, make_pi(CO_JF, LABEL_VAL));

    /* Code de la branche IF */
    pcode = merge_pcodes(pcode, gen_statseq(if_node->branches->sseq));
  
    /* Else ifs */
    while (elsifs) {
      /* Affectation du label courant */
      ADD_PI(pcode, make_pi(CO_LABEL, LABEL_VAL));
      ADD_PI(pcode, make_pi(CO_PUSH, CONST_OF_EXPR(elsifs->cond)));
      
      if (elsifs->next != NULL)
        GEN_NEW_LABEL("elseif");
      else if (if_node->else_branch)
        GEN_NEW_LABEL("else");
      
      ADD_PI(pcode, make_pi(CO_JF, LABEL_VAL));
      pcode =  merge_pcodes(pcode, gen_statseq(elsifs->sseq));
      elsifs = elsifs->next;
    }
  } else if (if_node->else_branch) {
    GEN_NEW_LABEL("else");
    ADD_PI(pcode, make_pi(CO_JF, LABEL_VAL));
    pcode = merge_pcodes(pcode, gen_statseq(if_node->branches->sseq));
    ADD_PI(pcode, make_pi(CO_LABEL, LABEL_VAL));
    pcode = merge_pcodes(pcode, gen_statseq(if_node->else_branch));
  } else {
    GEN_NEW_LABEL("endif");
    ADD_PI(pcode, make_pi(CO_JF, LABEL_VAL));
    pcode = merge_pcodes(pcode, gen_statseq(if_node->branches->sseq));
    ADD_PI(pcode, make_pi(CO_LABEL, LABEL_VAL));
  }

  return pcode;
}

pcode_t* gen_assign(assign_t* ass_node)
{
  pcode_t* pcode = NULL;

  ADD_PI(pcode, make_pi(CO_PUSH, CONST_OF_EXPR(ass_node->expr)));
  ADD_PI(pcode, make_pi(CO_STORE, vmval_from_ident(ass_node->desig)));

  return pcode;
}

pcode_t* gen_stat(stat_t* s)
{
  pcode_t* pcode_stat = NULL;

  switch (s->k) {
    case EXIT: ADD_PI(pcode_stat, gen_exit()); break;
    case ASSIGN: pcode_stat = gen_assign(s->u.assign); break;
    case IF: pcode_stat = gen_if(s->u.if_n); break;
    case WHILE: pcode_stat = gen_while(s->u.while_n); break;
    /* case PRINT: pcode_stat = gen_print(s->u.print_n); break; */
  }
  
  return pcode_stat;
}

pcode_t* gen_statseq(statseq_t* sseq)
{
  pcode_t *pcode = NULL,
          *pcode_stat;
  
  while (sseq) {
    pcode_stat = gen_stat(sseq->stat);
    pcode = merge_pcodes(pcode, pcode_stat);
    sseq = sseq->next;
  }

  return pcode;
}

pcode_t* gen_prog(prog_t* p)
{
  return gen_statseq(p->stats);
}
