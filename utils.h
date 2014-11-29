#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>

void* checked_malloc(size_t);

#define LL_INSERT_END(ll, el, start) do {         \
  if ((ll) == NULL)                               \
    return (el);                                  \
  else {                                          \
    while ((ll)->next != NULL) (ll) = (ll)->next; \
    (ll)->next = (el);                            \
    return (start);                               \
  }                                               \
} while (0)                                       

#endif /* _UTILS_H_ */
