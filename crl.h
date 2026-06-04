#ifndef CRL_H
#define CRL_H

#include <curl/curl.h>

typedef size_t (*crl_fn_t)(char * data, size_t sz, size_t n, void * ptr);

void crl_fetch(const char * host, const char * tkn, crl_fn_t rd, crl_fn_t wr) {
  CURL * curl = curl_easy_init();
  assert(curl);

  char url[10240]; snprintf(url, sizeof(url), "https://%s/v1/chat/completions", host);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, rd);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wr);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  char auth[10240];
  snprintf(auth, sizeof(auth), "Authorization: Bearer %s", tkn);

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
