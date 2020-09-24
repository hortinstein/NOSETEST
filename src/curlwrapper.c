#include <curl/curl.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curlwrapper.h"

#include "ll.h"
#include "debug.h"

void wrapper_curl_init(){
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
}

void wrapper_curl_free(){
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    uint8_t * ptr = (uint8_t *)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        /* out of memory! */
        DEBUG_ERROR("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int get_request(MemoryStruct * ms, char *url)
{
    CURL *curl_handle;
    CURLcode res;

    ms->memory = (uint8_t *)malloc(1); /* will be grown as needed by the realloc above */
    ms->size = 0;                   /* no data at this point */

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)ms);

    /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK)
    {
        DEBUG_ERROR("curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* cleanup curl stuff */
        curl_easy_cleanup(curl_handle);

        free(ms->memory);

        return FAILURE;
    }
    else
    {
        /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */

        DEBUG_PRINT("%lu bytes retrieved\n", (unsigned long)ms->size);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    
    //TODO add in
    //free(ms->memory);



    return SUCCESS;
}

int post_request(char *url, MemoryStruct *ms)
{
    if (!url || !ms) return FAILURE;
    
    CURL *curl;
    CURLcode res;

    
    /* get a curl handle */
    curl = curl_easy_init();
    if (curl)
    {
        /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
        curl_easy_setopt(curl, CURLOPT_URL, (char *)url);
        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ms->memory);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            DEBUG_ERROR("curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            /* always cleanup */
            curl_easy_cleanup(curl);
            return FAILURE;
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return SUCCESS;
}
