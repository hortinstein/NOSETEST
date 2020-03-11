
#include <monocypher.h>
#include "debug.h"

int enc(){

    uint8_t key        [32];    /* Random, secret session key  */ 
    uint8_t nonce      [24];    /* Use only once per key       */ 
    uint8_t plain_text [12] = "Lorem ipsum"; /* Secret message */ 
    uint8_t mac        [16];    /* Message authentication code */ 
    uint8_t cipher_text[12];              /* Encrypted message */ 
    arc4random_buf(key,   32); 
    arc4random_buf(nonce, 24); 
    crypto_lock(mac, cipher_text, key, nonce, plain_text, 
            sizeof(plain_text)); 
    /* Wipe secrets if they are no longer needed */ 
    crypto_wipe(plain_text, 12); 
    crypto_wipe(key, 32); 
    /* Transmit cipher_text, nonce, and mac over the network, 
    * store them in a file, etc. 
    */

   uint8_t       key        [32]; /* Same as the above        */ 
    uint8_t       nonce      [24]; /* Same as the above        */ 
    const uint8_t cipher_text[12]; /* Encrypted message        */ 
    const uint8_t mac        [16]; /* Received along with text */ 
    uint8_t       plain_text [12]; /* Secret message           */ 
    if (crypto_unlock(plain_text, key, nonce, mac, cipher_text, 12)) { 
        /* The message is corrupted. 
        * Wipe key if it is no longer needed, 
        * and abort the decryption. 
        */ 
        crypto_wipe(key, 32); 
    } else { 
        /* ...do something with the decrypted text here... */ 
        /* Finally, wipe secrets if they are no longer needed */ 
        crypto_wipe(plain_text, 12); 
        crypto_wipe(key, 32); 
    }
}