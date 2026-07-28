#ifndef RDR_H
#define RDR_H

#include "enc.h"

const char * rdr_ptr;
static size_t rdr_fn(char * data, size_t sz, size_t n, void * ptr) {
  assert(sz == 1 && "Expecting libcurl to pass size=1 as documented");

  int i;
  for (i = 0; *rdr_ptr && i < n; i++, rdr_ptr++, data++) *data = *rdr_ptr;
  return i;
}

static void rdr_reset() {
  rdr_ptr = enc_reset();
}

#endif
