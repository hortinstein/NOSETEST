
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
    uint8_t memory[];
} MemoryStruct;

//!COMMENT play around and see if you can get attribute packing to break
//!COMMENT PRETTYGOOD
typedef struct __attribute__((__packed__)) taskBytes
{
    uint16_t task_num;
    MemoryStruct taskArgs;
} TaskBytes;

//!COMMENT play around and see if you can get attribute packing to break
//!COMMENT PRETTYGOOD
typedef struct __attribute__((__packed__)) respBytes
{
    uint16_t task_num;
    MemoryStruct taskArgs;
} RespBytes;

typedef struct __attribute__((__packed__)) encryptedBytes
{
    uint8_t sender_pub_key[KEY_LEN]; /* senders public key */
    uint8_t nonce[NONCE_LEN];        /* Use only once per key       */
    uint8_t mac[MAC_LEN];            /* Message authentication code */
    MemoryStruct ciphertext;
} EncryptedBytes;

typedef struct __attribute__((__packed__)) deccryptedBytes
{
    MemoryStruct plainText;
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