#ifndef RDR_H
#define RDR_H

const char * rdr_post = " \
    \"type\": \"function\", \
    \"function\": { \
      \"name\": \"view_local_file\", \
      \"description\": \"Reads the text contents of a file relative to the workspace directory.\", \
      \"parameters\": { \
        \"type\": \"object\", \
        \"properties\": { \
          \"path\": { \
            \"type\": \"string\", \
            \"description\": \"The relative file path to read (e.g. 'src/index.js')\" \
          } \
        }, \
        \"required\": [\"path\"] \
      } \
    } \
  }], \
  \"messages\": [ \
    {\"role\": \"user\", \"content\": \"I want to find dead code in the current repository\"}, \
    { \"role\": \"assistant\",  \
      \"tool_calls\": [{ \
        \"id\": \"chatcmpl-tool-970c946da77d4697851ec2343f21c77d\", \
        \"type\": \"function\", \
        \"function\": { \
          \"name\": \"view_local_file\", \
          \"arguments\": \"{\\\"path\\\": \\\".\\\"}\" \
        } \
      } \
    ]}, \
    {\"role\": \"assistant\", \"content\": \"Let me start by exploring the repository structure to understand the codebase.\" }, \
    { \"role\": \"tool\", \
      \"tool_call_id\": \"chatcmpl-tool-970c946da77d4697851ec2343f21c77d\",  \
      \"name\": \"view_local_file\", \
      \"content\": \"main.c\\nmicroui.h\" \
    } \
  ] \
}";

size_t (*rdr_fn)(char * data, size_t sz, size_t n, void * ptr);

size_t rdr_p(char * data, size_t sz, size_t n, void * ptr) {
  assert(sz == 1 && "Expecting libcurl to pass size=1 as documented");

  int i;
  for (i = 0; *rdr_post && i < n; i++, rdr_post++, data++) *data = *rdr_post;
  return i;
}

const char * rdr_preamble_txt = "{ \
  \"model\": \"deepseek-v4-pro\", \
  \"stream\": true, \
  \"max_tokens\": 10240, \
  \"tools\": [{ ";
const char * rdr_preamble_ptr;
static size_t rdr_preamble(char * data, size_t sz, size_t n, void * ptr) {
  assert(sz == 1 && "Expecting libcurl to pass size=1 as documented");

  int i;
  for (i = 0; *rdr_preamble_ptr && i < n; i++, rdr_preamble_ptr++, data++) *data = *rdr_preamble_ptr;
  if (i == n) return i;

  rdr_fn = rdr_p;
  return i + rdr_p(data, sz, n - i, ptr);
}

void rdr_init() {
  rdr_fn = rdr_preamble;
  rdr_preamble_ptr = rdr_preamble_txt;
}

#endif
