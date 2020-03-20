#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H

#include <curl/curl.h>
#include "datatypes.h"

extern "C" void wrapper_curl_init();
extern "C" void wrapper_curl_free();
extern "C" int get_request(MemoryStruct * ms, char *url);
extern "C" int post_request(char *url, MemoryStruct * ms);

#endif