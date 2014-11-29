#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void* checked_malloc(size_t len)
{
  void* p = malloc(len);
  if (p == NULL) {
    fprintf(stderr, "Impossible d'allouer la mémoire\n");
    exit(EXIT_FAILURE);
  }
  return p;
}
