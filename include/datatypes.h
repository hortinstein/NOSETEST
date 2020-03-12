
#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>

#define KEY_LEN 32
#define NONCE_LEN 24
#define MAC_LEN 16

#define FAILURE 0 
#define SUCCESS 0

typedef struct memoryStruct {
  uint8_t size;
  char *memory;
} MemoryStruct;


typedef struct encryptedBytes{
    uint8_t sender_pub_key [NONCE_LEN];    /* senders public key */
    uint8_t nonce [NONCE_LEN];    /* Use only once per key       */ 
    uint8_t mac [MAC_LEN];    /* Message authentication code */ 
    uint8_t len; /* Secret message length*/
    uint8_t * cypher_text;  /* Secret message */ 
} EncryptedBytes;


typedef struct keyMat{
    uint8_t sender_pub_key [KEY_LEN];    /* Use only once per key       */ 
    uint8_t my_priv_key [KEY_LEN];    /* Message authentication code */  
    uint8_t shared_key [KEY_LEN];    /* derived shared key, not initiually populated */  
} KeyMat;


typedef struct localKeys{
    uint8_t my_pub_key [KEY_LEN];    /* Use only once per key       */ 
    uint8_t my_priv_key [KEY_LEN];    /* Message authentication code */  
} LocalKeys;


 
#endif