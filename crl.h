#ifndef CRL_H
#define CRL_H

#include "rdr.h"
#include "wrt.h"

const char * crl_host;
const char * crl_tkn;

void crl_fetch() {
  CURL * curl = curl_easy_init();
  assert(curl);

  char url[10240]; snprintf(url, sizeof(url), "https://%s/v1/chat/completions", crl_host);

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, rdr_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wrt_fn);
  // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

  char auth[10240];
  snprintf(auth, sizeof(auth), "Authorization: Bearer %s", crl_tkn);

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
