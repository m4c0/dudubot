#ifndef TLL_H
#define TLL_H

#include <string.h>

typedef struct tll_prop_s {
  const char * name;
  const char * type;
  const char * desc;
} tll_prop_t;
typedef struct tll_s {
  const char * name;
  const char * desc;
  char * (*func)(char *);
  tll_prop_t props[10];
  const char * reqs[10];

  struct tll_s * next;
} tll_t;

tll_t * tll_head;

tll_t * tll_find(const char * name) {
  for (tll_t * t = tll_head; t; t = t->next) {
    if (0 != strcmp(name, t->name)) continue;
    return t;
  }
  return NULL;
}

#endif
