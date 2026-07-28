#include "../tll_data.h"

#include <stdio.h>
#include <stdlib.h>

static char * buf;
static const char * exec(tll_call_t t) {
  printf("[\"expr\",\"getbufinfo()\",-1]\n");

  fgets(buf, 1024000, stdin);

  return buf;
}

EXPORT void dudubot_tool(tll_api_t * api) {
  if (api->magic != TLL_API_MAGIC_IN) return;
  api->magic = TLL_API_MAGIC_OUT;

  buf = malloc(1024000);

  *api->t = (tll_t) {
    .desc = "List open files.",
    .func = exec,
    .reqs = {},
    .props = {},
  };
}

