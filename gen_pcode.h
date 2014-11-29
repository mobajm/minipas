#ifndef _GEN_PCODE_H_
#define _GEN_PCODE_H_

#include "ir.h"
#include "ast.h"

/* pcode_t* gen_print(void); */
pi_t* gen_exit(void);
pcode_t* gen_while(while_t*);
pcode_t* gen_if(if_t*);
pcode_t* gen_assign(assign_t*);
pcode_t* gen_stat(stat_t*);
pcode_t* gen_statseq(statseq_t*);
pcode_t* gen_prog(prog_t*);

#endif /* _GEN_PCODE_H_ */
