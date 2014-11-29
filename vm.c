#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include "pile.h"
#include "vm.h"
#include "st.h"

extern st_t* glob_st;

Pile* VM_PILE;

void initialiser_machine(){
  VM_PILE=cree_pile();
}

label_t* get_label(pcode_t *pc){
  pcode_t *c;
  label_t *tete=NULL;
  c=pc;
  while(c != NULL){
    if(c->pi->codop == CO_LABEL){
      label_t* nv ;
      nv=(label_t*)malloc(sizeof(label_t));
      nv->pc = c;
      strcpy(nv->label,c->pi->param->u.label_name);
      nv->next=tete;
      tete=nv;		
    }	
  }
  return tete;
}

pcode_t* get_adCode(label_t* tete,char* label_name){
  pcode_t* find;
  label_t* c;
  c=tete;
  while(c!=NULL){
    if(strcmp(c->label,label_name)==0){	
      find=c->pc;
      break;
    }
    c=c->next;}
  return find;
}

void vider_pile(Pile *p){
  Pile *c;
  Pile *tmp;
  c=p;
  while(c!=NULL){tmp=c;c=c->next;free(tmp);}
}

void interpreter_pseudocode_instruction(pcode_t *pc){
  label_t* tete;
  entry_t* e;
  Element op1,op2,result;
  int cast;
  int increment_c = 1;
  tete = get_label(pc);
  pcode_t *c;
  c=pc;	
  while(c == NULL){
    switch(c->pi->codop){
      case CO_ADD :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=op1+op2;empiler(VM_PILE,result);	
      }break;
      case CO_DIV :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=op1/op2;cast=result;empiler(VM_PILE,cast);	
      }break;
      case CO_SLASH :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=op1/op2;empiler(VM_PILE,result);
      }break;
      case CO_MULT :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=op1*op2;empiler(VM_PILE,result);
      }break;
      case CO_SUB :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=op1-op2;empiler(VM_PILE,result);
      }break;
      case CO_OR :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);if(op1==1 || op2==1)result=1;else result=0;
        empiler(VM_PILE,result);
      }break;
      case CO_AND :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);if(op1==1 && op2==1)result=1;else result=0;
        empiler(VM_PILE,result);
      }break;
      case CO_MOD :{
        op1 = depiler(VM_PILE);op2=depiler(VM_PILE);result=((int)op1)%((int)op2);empiler(VM_PILE,result);
      }break;
      case CO_JMP :{
        pc = get_adCode(tete,c->pi->param->u.label_name);
        c=pc;
        increment_c=0;
      }break;
      case CO_JNE :{
        op1= depiler(VM_PILE);op2=depiler(VM_PILE);
        if(op1 != op2){		
          pc = get_adCode(tete,c->pi->param->u.label_name);
          c=pc;
          increment_c=0;}
      }break;
      case CO_JG :{
        op1= depiler(VM_PILE);op2=depiler(VM_PILE);
        if(op1 > op2){		
          pc = get_adCode(tete,c->pi->param->u.label_name);
          c=pc;
          increment_c=0;}
      }break;
      case CO_JGE :{
        op1= depiler(VM_PILE);op2=depiler(VM_PILE);
        if(op1 >= op2){		
          pc = get_adCode(tete,c->pi->param->u.label_name);
          c=pc;
          increment_c=0;}
      }break;
      case CO_JL :{
        op1= depiler(VM_PILE);op2=depiler(VM_PILE);
        if(op1 < op2){		
          pc = get_adCode(tete,c->pi->param->u.label_name);
          c=pc;
          increment_c=0;}
        case CO_JF :{
          op1= depiler(VM_PILE);
          if(op1 != 1){		
            pc = get_adCode(tete,c->pi->param->u.label_name);
            c=pc;
            increment_c=0;}
        }break;
        case CO_JLE :{
          op1= depiler(VM_PILE);op2=depiler(VM_PILE);
          if(op1 <= op2){		
            pc = get_adCode(tete,c->pi->param->u.label_name);
            c=pc;
            increment_c=0;}
        }break;
        case CO_DUP :{
          op1= depiler(VM_PILE);empiler(VM_PILE,op1);empiler(VM_PILE,op1);
        }break;
        case CO_SWAP:{
          op1=depiler(VM_PILE);op2=depiler(VM_PILE);empiler(VM_PILE,op1);empiler(VM_PILE,op2);
        }break;
        case CO_LOAD:{
          e = get_symbol(c->pi->param->u.var, glob_st);          
          op1 = e->val;
          empiler(VM_PILE,op1);
        }break;
        case CO_STORE:{
          op1=depiler(VM_PILE);			
          set_symbol_val(c->pi->param->u.var, op1, glob_st);
        }break;
        case CO_PUSH:{
          empiler(VM_PILE,c->pi->param->u._const);
        }break;
        case CO_POP:{
          depiler(VM_PILE);
        }break;
        case CO_PRNT:{
          op1=depiler(VM_PILE);printf("%lf",op1);
        }break;
        case CO_EXIT:{
          vider_pile(VM_PILE);
          printf("programme terminé ");			
          exit(0);
        }break;
        default: break;
		
      }	
	if(increment_c==1){c=c->next;}else{increment_c=1;}
    }
  }
}

