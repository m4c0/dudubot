#include <assert.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "crl.h"
#include "jsn.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <domain-name> <api-key>\n\n", *argv);
    fprintf(stderr, "Example: %s litellm.mycompany.com \"$API_KEY\"\n\n", *argv);
    return 1;
  }

  crl_host = argv[1];
  crl_tkn = argv[2];
  crl_fetch();

  const char * fini = wrt_msg->fini;

  if (!fini) {
    fputs("LLM ended without a concrete finish reason", stderr);
    return 1;
  }
  if (0 == strcmp(fini, "tool_calls")) {
    msg_t * tool = wrt_msg + 1;
    for (msg_tool_call_t * c = wrt_msg->calls; c->id; c++) {
      tll_t * t = tll_find(c->name);
      assert(t && "tool not found"); // discard message and try again?

      assert(0 == strcmp(c->name, "view_local_file"));

      char * json = jsn_decode(c->args);

      json_object_t * root = jsn_parse_object(json, strlen(json));
      assert(root && "invalid tool call args");

      const char * path = jsn_str(jsn_find_element(root, "path"));
      assert(path && "missing 'path' in 'view_local_file' arguments");

      assert(0 == strcmp(path, "."));
      *tool++ = (msg_t) {
        .role = "tool",
        .call = strdup(c->id),
        .name = "view_local_file",
        .cont = "main.c\\nmicroui.h",
      };
    }
  }

  crl_fetch();
}

