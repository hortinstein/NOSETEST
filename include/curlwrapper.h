#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H

#include <curl/curl.h>

extern "C" int get_request(char * url);

int post_request(char * url);

#endif