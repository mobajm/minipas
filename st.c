#include "st.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

#define ST_SIZE 113

static buckets_t* add_to_buckets(entry_t* e, buckets_t* bucks)
{
  buckets_t* new = checked_malloc(sizeof *new);
  buckets_t* bucks_start = bucks;
      
  new->data = e;
  new->next = NULL;

  LL_INSERT_END(bucks, new, bucks_start);
}

static int fct_hash(char* name)
{
  int accum = 0;
  char *s = name;

  while (*s) {
    accum <<= 1;
    accum += (unsigned) (*s++ & 0xFF);
  }
  
  return accum % ST_SIZE;
}

st_t* make_st(void)
{
  int i ;
  st_t* symbt = checked_malloc(sizeof *symbt);
  
  symbt->size = ST_SIZE;
  symbt->data = checked_malloc(ST_SIZE * sizeof *(symbt->data));

  for(i = 0; i < ST_SIZE; i++)
    symbt->data[i] = NULL; 

  return symbt;
}

entry_t* make_entry(char* name, type_t* ty, int ro, double val)
{
  entry_t* e = checked_malloc(sizeof *e);

  e->nom = name;
  e->ty = ty;
  e->ro = e->is_init = ro;
  e->val = val;

  return e;
}

void add_symbol(entry_t* e, st_t* st){
  int key;
  buckets_t* buck;

  key = fct_hash(e->nom);
  
  buck = st->data[key];
  buck = add_to_buckets(e, buck);
  
  st->data[key] = buck;
}

entry_t* get_symbol(char* name, st_t* st)
{
  int key = fct_hash(name);
  buckets_t* bucks = st->data[key];

  if (bucks == NULL)
    return NULL;
  else {
    while (bucks) {
      if (!strcmp(bucks->data->nom, name))
        return bucks->data;
      bucks = bucks->next;
    }
    return NULL;
  }
}

void set_symbol_val(char* nom, double val, st_t* st)
{
  entry_t* e = get_symbol(nom, st);
  if (e != NULL && e->ro != 1) {
    e->val = val;
    e->is_init = 1;
  }
}
