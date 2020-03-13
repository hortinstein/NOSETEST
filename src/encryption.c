#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <monocypher.h>

#include "encryption.h"
//todo srand errors and no checking
int gen_rdm_bytestream (LocalKeys * km, size_t num_bytes)
{
  if (!km) return FAILURE;
  srand ((unsigned int) time (NULL));
  unsigned char *stream = (unsigned char *)malloc (num_bytes);
  size_t i;

  for (i = 0; i < num_bytes; i++)
  {
    stream[i] = rand ();
  }

  memcpy(km->my_priv_key,stream,num_bytes);
  return SUCCESS;
}

int gen_keys(LocalKeys * km){
    if (!km) return FAILURE;
    gen_rdm_bytestream(km,KEY_LEN); 
    
    crypto_key_exchange_public_key(km->my_pub_key, km->my_priv_key); 
    return SUCCESS;
}

int derive_session_key(KeyMat * km, uint8_t * my_priv_key,  uint8_t * sender_pub_key){
    if (!km || !my_priv_key || !sender_pub_key) return FAILURE;


    return SUCCESS;
}

int enc(EncryptedBytes * eb , KeyMat * km, uint8_t * pt ){
    if (!eb || !km || !pt) return FAILURE;
    // uint8_t nonce      [24];    /* Use only once per key       */ 
    // uint8_t mac        [16];    /* Message authentication code */ 
    // arc4random_buf(nonce, 24); 
    // crypto_lock(mac, cipher_text, key, nonce, plain_text, 
    //         sizeof(plain_text)); 
    return SUCCESS;
}

