#ifndef CRL_H
#define CRL_H

#include "rdr.h"
#include "utl.h"
#include "wrt.h"

#ifdef _WIN32
#  include <winhttp.h>
#else
#  include <curl/curl.h>
#endif

#define CRL_URL_ENV_NAME "DUDUBOT_URL"
#define CRL_URL_ENV_DEF  "https://api.deepseek.com/chat/completions"

#ifdef _WIN32
static wchar_t * crl_url;

static void crl_actually_fetch(const char * auth) {
  if (!crl_url) crl_url = utl_env_wc(CRL_URL_ENV_NAME, CRL_URL_ENV_DEF);

  (void)rdr_fn;
  (void)wrt_fn;

  wchar_t host[256];
  wchar_t path[2048];

  URL_COMPONENTS comps = {
    .dwStructSize     = sizeof(URL_COMPONENTS),
    .lpszHostName     = host,
    .dwHostNameLength = 256,
    .lpszUrlPath      = path,
    .dwUrlPathLength  = 2048,
  };
  assert(WinHttpCrackUrl(crl_url, 0, 0, &comps));
  assert(comps.nPort == 443);

  HINTERNET sess = WinHttpOpen(L"dudubot/0.1", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
  assert(sess);

  HINTERNET conn = WinHttpConnect(sess, comps.lpszHostName, 443, 0);
  assert(conn);

  // TODO: is there a "accept: json" of sorts?
  HINTERNET req = WinHttpOpenRequest(conn,
      L"POST", comps.lpszUrlPath, NULL,
      WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
      WINHTTP_FLAG_SECURE);
  assert(req);

  wchar_t * wauth = utl_mbstowcs(auth);
  WinHttpAddRequestHeaders(req, wauth, -1, WINHTTP_ADDREQ_FLAG_ADD);
  WinHttpAddRequestHeaders(req, L"Content-Type: application/json", -1, WINHTTP_ADDREQ_FLAG_ADD);

  int req_len = strlen(rdr_ptr);
  assert(WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, rdr_ptr, req_len, req_len, 0));

  assert(WinHttpReceiveResponse(req, NULL)); 

  char resp[4096];
  DWORD resp_sz;
  while (WinHttpReadData(req, resp, sizeof(resp), &resp_sz)) {
    if (resp_sz == 0) break;
    wrt_fn(resp, (int)resp_sz, 1, NULL);
  }

  free(wauth);

  WinHttpCloseHandle(req);
  WinHttpCloseHandle(conn);
  WinHttpCloseHandle(sess);
}
#else
const char * crl_url;

static void crl_actually_fetch(const char * auth) {
  if (!crl_url) crl_url = utl_env(CRL_URL_ENV_NAME, CRL_URL_ENV_DEF);

  CURL * curl = curl_easy_init();
  assert(curl);

  curl_easy_setopt(curl, CURLOPT_URL, crl_url);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, rdr_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrt_fn);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  struct curl_slist * hdrs = NULL;
  hdrs = curl_slist_append(hdrs, "Content-type: application/json");
  hdrs = curl_slist_append(hdrs, auth);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) printf("err: %s\n", curl_easy_strerror(res));

  curl_slist_free_all(hdrs);

  curl_easy_cleanup(curl);
}
#endif

void crl_fetch(void) {
  char * api_key = getenv("DUDUBOT_API_KEY");
  assert(api_key && "Missing environment 'DUDUBOT_API_KEY'");

  char auth[10240];
  snprintf(auth, sizeof(auth), "Authorization: Bearer %s", api_key);

  crl_actually_fetch(auth);
}

#endif
