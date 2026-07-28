#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

void log_v(const char * msg, va_list args);

__attribute__((format(printf, 1, 2)))
static inline int log_error(const char * msg, ...) {
  va_list args;
  va_start(args, msg);
  log_v(msg, args);
  va_end(args);
  return 1;
}

__attribute__((format(printf, 1, 2)))
static inline int log_info(const char * msg, ...) {
  va_list args;
  va_start(args, msg);
  log_v(msg, args);
  va_end(args);
  return 0;
}

#endif

