
#ifndef DATATYPES_H
#define DATATYPES_H

#include <stdint.h>

#define KEY_LEN 32
#define NONCE_LEN 24
#define MAC_LEN 16

#define FAILURE -1
#define SUCCESS 0

typedef struct  memoryStruct
{
    uint32_t size;
    uint8_t * memory;
} MemoryStruct;

typedef struct __attribute__((__packed__)) taskBytes
{
    uint8_t task_num;
    uint32_t len;                  /* length of the args */ 
    uint8_t *task_args;            /* Secret message */
} TaskBytes;

typedef struct __attribute__((__packed__)) encryptedBytes
{
    uint8_t sender_pub_key[KEY_LEN]; /* senders public key */
    uint8_t nonce[NONCE_LEN];        /* Use only once per key       */
    uint8_t mac[MAC_LEN];            /* Message authentication code */
    uint32_t len;                     /* Secret message length*/
    uint8_t *cypher_text;            /* Secret message */
} EncryptedBytes;

typedef struct __attribute__((__packed__)) deccryptedBytes
{
    uint32_t len;         /* Secret message length*/
    uint8_t *plain_text; /* Secret message */
} DecryptedBytes;

typedef struct keyMat
{
    uint8_t sender_pub_key[KEY_LEN]; /* Use only once per key       */
    uint8_t my_priv_key[KEY_LEN];    /* Message authentication code */
    uint8_t shared_key[KEY_LEN];     /* derived shared key, not initiually populated */
} KeyMat;

typedef struct localKeys
{
    uint8_t my_pub_key[KEY_LEN];  /* Use only once per key       */
    uint8_t my_priv_key[KEY_LEN]; /* Message authentication code */
} LocalKeys;

#endif