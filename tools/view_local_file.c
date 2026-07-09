#include "../tll_data.h"

#include <stdio.h>

static char * exec(char * args) {
  puts(args);
  return "tool not working";
}

void dudubot_tool(tll_t * t) {
  *t = (tll_t) {
    .desc = "Reads the text contents of a file relative to the workspace directory.",
    .func = exec,
    .reqs = { "path" },
    .props = {{
      .name = "path",
      .type = "string",
      .desc = "The relative file path to read (e.g. 'src/index.js')",
    }},
  };
}
