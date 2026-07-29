#include "../jsn.h"
#include "../tll_data.h"
#include "../str.h"

#include <stdio.h>
#include <stdlib.h>

static char * buf;
static const char * exec(tll_call_t t) {
  int bufnr = jsn_atoi(jsn_find_element(t.json, "bufnr"));
  if (bufnr <= 0) return "missing valid 'bufnr' in arguments";

  const char * start = jsn_str(jsn_find_element(t.json, "start"));
  if (!start || atoi(start) <= 0) start = "1";
  const char * end = jsn_str(jsn_find_element(t.json, "end"));
  if (!end || atoi(end) <= 0) end = "'$'";

  printf("[\"call\",\"bufload\",[%d]]\n", bufnr);
  fflush(stdout);
  printf("[\"expr\",\"getbufline(%d, %s, %s)\",-1]\n", bufnr, start, end);
  fflush(stdout);

  if (!buf) buf = malloc(1024000);
  if (!fgets(buf, 1024000, stdin)) return "tool failed to run";

  json_array_element_t * arr = jsn_arr(jsn_parse(buf, strlen(buf)));
  if (!arr) return "VIM returned a non-array message";

  if (jsn_atoi(arr->value) != -1) return "VIM returned an invalid message";

  int line = atoi(start);
  str_bld_t * res = NULL;
  for (arr = jsn_arr(arr->next->value); arr; arr = arr->next) {
    snprintf(buf, 1024, "%d: ", line++);
    str_bld_cat(&res, buf);
    str_bld_cat(&res, jsn_str(arr->value));
    str_bld_cat(&res, "\n");
  }
  if (!res) return "";
  return str_bld_flush(&res);
}

EXPORT void dudubot_tool(tll_api_t * api) {
  if (api->magic != TLL_API_MAGIC_IN) return;
  api->magic = TLL_API_MAGIC_OUT;

  *api->t = (tll_t) {
    .desc = "Retrieves lines from an open buffer",
    .func = exec,
    .reqs = {"bufnr"},
    .props = {{
      .name = "bufnr",
      .type = "integer",
      .desc = "Buffer number",
    }, {
      .name = "start",
      .type = "integer",
      .desc = "First line to get (defaults to first of the file)",
    }, {
      .name = "end",
      .type = "integer",
      .desc = "Last line to get (defaults to last of the file)",
    }},
  };
}

