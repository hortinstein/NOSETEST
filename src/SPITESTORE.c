
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>

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
    
    //convert the binary to ascii
    b64_km.memory = base64_encode((const unsigned char *)km->my_pub_key,KEY_LEN,(size_t*)&b64_km.size);
    DEBUG_PRINT("%s",b64_km.memory);

    post_request((char *)PP_KEY_URL, &b64_km);

    //TODO take out here
    free(b64_km.memory);
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int recv_keymat(keyMat * km,localKeys *lk){
    MemoryStruct b64_km;
    MemoryStruct sender_key;
    if (!km || !lk) goto fail;
    
    b64_km.memory = NULL;
    get_request(&b64_km,(char *)PP_KEY_URL);
    
    DEBUG_PRINT("%s",b64_km.memory);

    sender_key.memory = base64_decode((const unsigned char *)b64_km.memory,(size_t)b64_km.size,(size_t*)&sender_key.size);
    
    //copy their keymat into the km structure
    //what should i be checking for here
    //TODO
    memcpy(km->sender_pub_key,sender_key.memory,KEY_LEN);
    
    //copy my private key
    memcpy(km->my_priv_key,lk->my_priv_key,KEY_LEN);
    
    //derive the session key
    derive_session_key(km);
    print_key(km->shared_key,(char *)"shared key: ");
    
    free(b64_km.memory);
    free(sender_key.memory);
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int get_task(keyMat * km){
if (!km) goto fail;
   
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int send_result(keyMat * km){
if (!km) goto fail;

    return SUCCESS;
fail:
    return FAILURE;
}

int main()
{
    wrapper_curl_init();
    
    localKeys me;
    keyMat session;  

    DEBUG_PRINT("SPITESTORE\n");
 
    //generates my local keys
    gen_keys(&me);

    //send my keymat
    send_keymat(&me);
    
    //recv their keymat
    recv_keymat(&session,&me);
    
    //task loop
    while (1){
        sleep(10);
        get_task(&session);

        send_result(&session);
    }
    wrapper_curl_free();
   
}