#include "cht.h"

void log_v(const char * msg, va_list args) {
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
}

static int read_msg(void) {
  printf("> "); fflush(stdout);

  char buf[10240];
  if (!fgets(buf, 10240, stdin)) return 1;
  buf[strlen(buf) - 1] = 0;

  if (0 == strlen(buf)) return read_msg();

  if (0 == strcmp(buf, "."   )) return 0;
  if (0 == strcmp(buf, "end" )) return 1;
  if (0 == strcmp(buf, "exit")) return 1;
  if (0 == strcmp(buf, "quit")) return 1;

  if (0 == strncmp(buf, "tool ", 5)) {
    if (msg_head) printf("WARNING: changing tools after conversation started nukes token caching\n");
    tll_load(buf + 5);
    return read_msg();
  }
  if (0 == strncmp(buf, "load ", 5)) {
    if (msg_load(buf + 5)) printf("failed to load messages\n");
    return read_msg();
  }
  if (0 == strncmp(buf, "save ", 5)) {
    if (msg_save(buf + 5)) printf("failed to save messages\n");
    return read_msg();
  }

  if (0 == strcmp(buf, "a")) {
    char line[1024];
    char * ptr = buf;
    while (1) {
      printf(". "); fflush(stdout);

      if (!fgets(line, 1024, stdin)) return 1;
      if (0 == strcmp(line, ".\n")) break;
      for (int i = 0; line[i] && i < 1024 && ptr < buf + 10240; i++) *ptr++ = line[i];
    }
    *ptr = 0;
  }

  msg_t * msg = msg_alloc();
  msg->role = "user";
  msg->cont = strdup(buf);
  return 0;
}

static int end() {
  tll_purge();
  return 0;
}

int main(int argc, char ** argv) {
  for (int i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "-")) {
      assert(i + 1 == argc && "stdin marker should be last");
      if (msg_load_file(stdin)) return 1;
      wrt_quiet = 1;
      cht_loop(NULL);
      return end();
    }
    else if (0 == strcmp(argv[i], ".")) {
      assert(i + 1 == argc && "run marker should be last");
      cht_loop(NULL);
    }
    else if (msg_load(argv[i])) return 1;
  }

#ifdef _WIN32
  char * tmp = getenv("TEMP");
#else
  char * tmp = getenv("TMPDIR");
  if (!tmp) tmp = "/tmp";
#endif

  char session[PATH_MAX];
  snprintf(session, PATH_MAX, "%s/dudubot-%lld.chat", tmp, (long long)time(NULL));
  do {
    if (read_msg()) return 0;
  } while (cht_loop(session));

  return end();
}
