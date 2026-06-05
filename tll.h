#ifndef TLL_H
#define TLL_H

#include "json.h"

typedef struct tll_arg_s {
  char * name;
  char * value;
} ttl_arg_t;
typedef struct tll_args_s {
  ttl_arg_t list[10];
} ttl_args_t;
typedef struct tll_prop_s {
  const char * name;
  const char * type;
  const char * desc;
} tll_prop_t;
typedef struct tll_s {
  const char * name;
  const char * desc;
  tll_prop_t props[10];
  const char * reqs[10];
} tll_t;

tll_t tll_list[] = {
  {
    .name = "view_local_file",
    .desc = "Reads the text contents of a file relative to the workspace directory.",
    .reqs = { "path" },
    .props = {{
      .name = "path",
      .type = "string",
      .desc = "The relative file path to read (e.g. 'src/index.js')",
    }},
  },
  {}
};

tll_t * tll_find(const char * name) {
  for (tll_t * t = tll_list; t->name; t++) {
    if (0 != strcmp(name, t->name)) continue;
    return t;
  }
  return NULL;
}

void ttl_parse_args(tll_t * t, const char * str, ttl_args_t * args) {
  for (int i = 0; i < 10; i++) {
    if (args->list[i].name ) free(args->list[i].name );
    if (args->list[i].value) free(args->list[i].value);
  }
}

#endif
