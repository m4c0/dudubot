#include "cht.h"

jse_t log_buf;
void log_v(const char * msg, va_list args) {
  jse_reset(&log_buf, 10240);

  jse_cat(&log_buf, "[");
  jse_cat_str(&log_buf, "ex");
  jse_cat(&log_buf, ",");

  char buf[1024];
  vsnprintf(buf, 1024, msg, args);

  char buf2[1024];
  snprintf(buf2, 1024, "echom '%s'", buf);
  jse_cat_str(&log_buf, buf2);

  jse_cat(&log_buf, "]");

  printf("%s\n", jse_finish(&log_buf));
  printf("[\"redraw\",\"\"]\n");
}

void wrt_log_cont(const char * str) {}
void wrt_log_reas(const char * str) {}

int main(int argc, char ** argv) {
  if (argc != 2) return log_error("Expected usage: %s <chat-file>", argv[0]);

  msg_load(argv[1]);

  cht_loop(argv[1]);

  printf("[\"ex\",\"edit %s\"]\n", argv[1]);
  printf("[\"normal\",\"G\"]\n");
  printf("[\"redraw\",\"\"]\n");

  tll_purge();
  return 0;
}
