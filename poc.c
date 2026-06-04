#include <assert.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "crl.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <domain-name> <api-key>\n\n", *argv);
    fprintf(stderr, "Example: %s litellm.mycompany.com \"$API_KEY\"\n\n", *argv);
    return 1;
  }

  crl_host = argv[1];
  crl_tkn = argv[2];
  crl_fetch();
}

