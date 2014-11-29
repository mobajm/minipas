#include "pile.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

Pile* cree_pile(){
	
  Pile *p;
  p = checked_malloc(sizeof(Pile));

  return p; 		
}


int pile_vide(Pile *p){
  if(p == NULL){return 1;}
  return 0;
}

Pile* empiler(Pile *p,Element element){
	
  Pile *nv;
  nv =checked_malloc(sizeof(Pile));
  nv->element=element;
  nv->next=p;
  p=nv;

  return p;
}

Element depiler(Pile *p){
  Element element;
  Pile *tmp;
  tmp=p;
  element = p->element;
  p=p->next;
  free(tmp);

  return element;
}

void etat_pile(Pile *p){
  Pile *c;
  c=p;
  while(c!=NULL){printf(" %lf  ||",c->element);c=c->next;}
}

