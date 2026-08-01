#include "../jsn.h"
#include "../tll_data.h"
#include "../str.h"

#include <stdio.h>
#include <stdlib.h>

static char * buf;
static const char * exec(tll_call_t t) {
  printf("[\"expr\",\"map(getbufinfo({'buflisted':1}), 'v:val.bufnr .. \\\": \\\" .. v:val.name')\",-1]\n");
  fflush(stdout);

  if (!buf) buf = malloc(1024000);
  if (!fgets(buf, 1024000, stdin)) return "tool failed to run";

  json_array_element_t * arr = jsn_arr(jsn_parse(buf, strlen(buf)));
  if (!arr) return "VIM returned a non-array message";

  if (jsn_atoi(arr->value) != -1) return "VIM returned an invalid message";

  str_bld_t * res = NULL;
  for (arr = jsn_arr(arr->next->value); arr; arr = arr->next) {
    const char * bufname = jsn_str(arr->value); 

    // These makes the LLM **VERY** confused.
    if (strstr(bufname, ".chat")) continue;

    str_bld_cat(&res, bufname);
    str_bld_cat(&res, "\n");
  }
  if (!res) return "No files are open. Ask the user for help.";
  return str_bld_flush(&res);
}

EXPORT void dudubot_tool(tll_api_t * api) {
  if (api->magic != TLL_API_MAGIC_IN) return;
  api->magic = TLL_API_MAGIC_OUT;

  *api->t = (tll_t) {
    .desc = "List available files with their buffer number.",
    .func = exec,
    .reqs = {},
    .props = {},
  };
}

