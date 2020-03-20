#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <monocypher.h>

#include "encryption.h"

int gen_randon_nonce (EncryptedBytes * eb){
  if (!eb) return FAILURE;

  srand ((unsigned int) time (NULL));
  unsigned char stream [NONCE_LEN];
  
  size_t i;

  for (i = 0; i < NONCE_LEN; i++)
  {
    stream[i] = rand ();
  }

  memcpy(eb->nonce,stream,NONCE_LEN);
  return SUCCESS;
}


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

  ///!_SKELETON
  ///!_SKELETON

  ///!_SOLUTION
  free(stream);
  ///!_SOLUTION
  return SUCCESS;
}
///!_SOLUTION

//if you are reading this what is wrong with this function
void eb_init(EncryptedBytes * eb){
  if (!eb) return;
  eb->cypher_text = NULL;
  eb->len = 0;
}

//and if you are reading this what is wrong with this function
void eb_free(EncryptedBytes * eb){
  if (!eb) return;
  free(eb->cypher_text);
  eb->len = 0;
}

//and if you are reading this what is wrong with this function
void db_init(DecryptedBytes * db){
  if (!db) return;
  db->plain_text = NULL;
  db->len = 0;
}

//and if you are reading this what is wrong with this function
void db_free(DecryptedBytes * db){
  if (!db) return;
  free(db->plain_text);
  db->len = 0;
}


int gen_keys(LocalKeys * km){
    if (!km) return FAILURE;
    gen_rdm_bytestream(km,KEY_LEN); 
    
    crypto_key_exchange_public_key(km->my_pub_key, km->my_priv_key); 
    return SUCCESS;
}

int derive_session_key(KeyMat * km){
    if (!km) return FAILURE;

    crypto_key_exchange(km->shared_key, km->my_priv_key, km->sender_pub_key);
    return SUCCESS;
}

int enc(EncryptedBytes * eb , KeyMat * km, DecryptedBytes * db){
    if (!eb || !km || !db || eb->cypher_text != NULL) return FAILURE;
    DEBUG_PRINT("encrypyting %u bytes...",db->len);

    eb->cypher_text = (uint8_t *) malloc(db->len);
    ///!_SKELETON
    //!_ //forgetting something?
    ///!_SKELETON

    ///!_SOLUTION
    gen_randon_nonce(eb);
    ///!_SOLUTION

    crypto_lock(eb->mac, eb->cypher_text, km->shared_key, eb->nonce, db->plain_text, db->len); 
    eb->len = db->len;
    return SUCCESS;
}

int dec(DecryptedBytes * db, KeyMat * km, EncryptedBytes * eb){
    if (!eb || !km || !db) return FAILURE;
    DEBUG_PRINT("decrypting %u bytes...",eb->len);

    db->plain_text = (uint8_t *) malloc(eb->len);
    db->len = eb->len;

    crypto_unlock(db->plain_text, km->shared_key, eb->nonce, eb->mac, eb->cypher_text, eb->len);
    
    return SUCCESS;
}