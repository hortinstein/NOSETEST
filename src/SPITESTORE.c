
#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include "ll.h"
#include "debug.h"
#include "datatypes.h"
#include "curlwrapper.h"
#include "listwrapper.h"
#include "encryption.h"
}
 

//this function send the key material to the server
int send_keymat(localKeys * km){
    MemoryStruct b64_km;
    if (!km) goto fail;
    

    //put it intp
    //convert the binary to ascii
    b64_km.memory = base64_encode((const unsigned char *)km,sizeof(localKeys),(size_t*)&b64_km.size);
    DEBUG_PRINT("%s",b64_km.memory);

    post_request((char *)PP_URL, &b64_km);

    //TODO take out here
    free(b64_km.memory);
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int recv_keymat(keyMat * km,localKeys *lk){
    if (!km || !lk) goto fail;
    
    MemoryStruct ms;
    wrapper_curl_init();
    get_request(&ms,(char *)"");
    wrapper_curl_free();

    return SUCCESS;
fail:
    return FAILURE;
}



int main()
{
    localKeys me;
    //keyMat session;  

    //generates my local keys
    gen_keys(&me);
    send_keymat(&me);

    DEBUG_PRINT("SPITESTORE\n");
    
}