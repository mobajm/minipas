#ifndef PILE_H
#define PILE_H

#define TAILLEPILEMAX 100

typedef double Element ;



typedef struct pile{
	Element element;
	struct pile *next;
} Pile;

Pile* cree_pile();

int pile_vide(Pile *p);

Pile* empiler(Pile *p,Element element);

Element depiler(Pile *p);

void etat_pile(Pile *p);




#endif
