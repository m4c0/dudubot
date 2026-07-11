#include "tll.h"

#include <assert.h>

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s <tool-name> <json>\n", argv[0]);
    return 1;
  }

  if (tll_load(argv[1])) return 1;

  tll_t * t = tll_find(argv[1]);
  assert(t);

  puts(t->func(argv[2]));

  return 0;
}
