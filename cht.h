#ifndef CHT_H
#define CHT_H

#include "crl.h"

static int cht_loop(const char * session) {
  rdr_reset();
  wrt_reset();
  if (session) msg_save(session);

  crl_fetch();

  wrt_flush();
  if (session) msg_save(session);

  const char * fini = wrt_msg->fini;

  if (!fini) return log_info("LLM ended without a concrete finish reason");
  log_info("finish reason: %s", wrt_msg->fini);

  if (0 == strcmp(fini, "stop")) return 1;
  if (0 == strcmp(fini, "tool_calls")) {
    for (msg_tool_call_t * c = wrt_msg->calls; c; c = c->next) {
      *msg_alloc() = (msg_t) {
        .role = "tool",
        .call = strdup(c->id),
        .name = strdup(c->name),
        // expecting tools to return either a malloc'd string or a literal
        .cont = tll_exec(c->id, c->name, c->args),
      };
    }
    return cht_loop(session);
  }

  return 0;
}

#endif
