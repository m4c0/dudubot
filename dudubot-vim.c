#include "cht.h"

jse_t log_buf;
void log_v(const char * msg, va_list args) {
  jse_reset(&log_buf, 10240);

  jse_cat(&log_buf, "[");
  jse_cat_str(&log_buf, "ex");
  jse_cat(&log_buf, ",");
  jse_cat_str(&log_buf, "echom");
  jse_cat(&log_buf, ",");

  char buf[1024];
  vsnprintf(buf, 1024, msg, args);
  jse_cat_str(&log_buf, buf);

  jse_cat(&log_buf, "]");

  printf("%s\n", jse_finish(&log_buf));
}

void wrt_log_cont(const char * str) {}
void wrt_log_reas(const char * str) {}

static char * vim_cmd;
static int read_msg(const char * session) {
  if (!vim_cmd) vim_cmd = malloc(1024000);
  if (!fgets(vim_cmd, 1024000, stdin)) return 1;

  json_array_element_t * arr = jsn_arr(json_parse(vim_cmd, strlen(vim_cmd)));
  assert(arr);

  arr = arr->next;
  assert(arr);

  const char * prompt = jsn_str(arr->value);
  if (prompt) {
    msg_alloc_user(prompt);
    return cht_loop(session);
  }

  return 1;
}

int main(int argc, char ** argv) {
  if (argc != 2) return log_error("Expected usage from vim: %s <chat-file>", argv[0]);

  msg_load(argv[1]);

  while (read_msg(argv[1])) {
    printf("[\"ex\",\"edit %s\"]\n", argv[1]);
    fflush(stdout);
    printf("[\"redraw\",\"\"]\n");
    fflush(stdout);
  }

  tll_purge();
  return 0;
}
