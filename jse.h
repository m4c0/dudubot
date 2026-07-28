#ifndef JSE_H
#define JSE_H

#include <stdlib.h>

typedef struct jse_s {
  char * buf;
  char * ptr;
  char * end;
} jse_t;

jse_t jse_alloc(unsigned sz) {
  jse_t res = {0};
  res.ptr = res.buf = malloc(sz);
  res.end = res.ptr + sz;
  return res;
}
void jse_reset(jse_t * t, unsigned sz) {
  if (!t->buf) *t = jse_alloc(sz);
  t->ptr = t->buf;
}
const char * jse_finish(jse_t * t) {
  *t->ptr = 0;
  return t->buf;
}

void jse_cat(jse_t * t, const char * str) {
  for (; *str && t->end > t->ptr + 1; str++, t->ptr++) *t->ptr = *str;
}
void jse_cat_str(jse_t * t, const char * str) {
  jse_cat(t, "\"");
  for (; *str && t->end > t->ptr + 1; str++) {
    switch (*str) {
      case '\"': jse_cat(t, "\\\""); break;
      case '\\': jse_cat(t, "\\\\"); break;
      case '\n': jse_cat(t, "\\n" ); break;
      case '\r': jse_cat(t, "\\r" ); break;
      case '\t': jse_cat(t, "\\t" ); break;
      default: *t->ptr++ = *str; break;
    }
  }
  jse_cat(t, "\"");
}
void jse_cat_k(jse_t * t, const char * k) {
  jse_cat_str(t, k);
  jse_cat(t, ":");
}
void jse_cat_kv(jse_t * t, const char * k, const char * v) {
  jse_cat_k(t, k);
  jse_cat_str(t, v);
}
void jse_cat_kv_comma(jse_t * t, const char * k, const char * v) {
  jse_cat_kv(t, k, v);
  jse_cat(t, ",");
}

#endif
