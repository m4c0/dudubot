#ifndef MSG_H
#define MSG_H

typedef struct msg_tool_call_s {
  const char * id;
  char * name;
  char * args;
} msg_tool_call_t;
typedef struct msg_s {
  const char * role;
  const char * call;
  const char * name;
  const char * fini;
  char * cont;
  char * reas;
  msg_tool_call_t * calls;
} msg_t;

msg_t msg_convo[10000] = {0};

int msg_save(const char * name) {
  printf("would save in %s\n", name);
  return 0;
}
int msg_load(const char * name) {
  printf("would load from %s\n", name);
  return 0;
}


#endif
