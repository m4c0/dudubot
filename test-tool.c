#include "log.h"
#include "tll.h"

#include <assert.h>

void log_v(const char * msg, va_list args) {
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
}

void wrt_log_cont(const char * str) {
  fprintf(stdout, "%s", str);
  fflush(stdout);
}
void wrt_log_reas(const char * str) {
  fprintf(stderr, "%s", str);
  fflush(stderr);
}


int main(int argc, char ** argv) {
  if (argc != 3) return log_error("usage: %s <tool-name> <json>\n", argv[0]);

  tll_dudubot_exe = "./dudubot";
  if (tll_load(argv[1])) return 1;

  puts(tll_exec("test-tool", argv[1], argv[2]));

  return 0;
}
