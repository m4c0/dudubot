#include "../jsn.h"
#include "../tll_data.h"

#include <stdio.h>
#include <stdlib.h>

static char * buf;
static const char * exec(tll_call_t t) {
  printf("[\"expr\",\"map(getbufinfo(), 'v:val.name')\",-1]\n");
  fflush(stdout);

  if (!buf) buf = malloc(1024000);
  if (!fgets(buf, 1024000, stdin)) return "tool failed to run";

  json_value_t * root = json_parse(buf, strlen(buf));
  json_array_element_t * arr = jsn_arr(root);
  if (!arr) return "VIM returned a non-array message";

  if (jsn_atoi(arr->value) != -1) return "VIM returned an invalid message";

  free(root);
  return buf;
}

EXPORT void dudubot_tool(tll_api_t * api) {
  if (api->magic != TLL_API_MAGIC_IN) return;
  api->magic = TLL_API_MAGIC_OUT;

  *api->t = (tll_t) {
    .desc = "List open files.",
    .func = exec,
    .reqs = {},
    .props = {},
  };
}

