
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <monocypher.h>
#include "datatypes.h"
#include "debug.h"



extern "C" int gen_keys(LocalKeys * km);
extern "C" int derive_session_key(KeyMat * km, uint8_t * my_priv_key,  uint8_t * sender_pub_key);
extern "C" int enc(EncryptedBytes eb , KeyMat * km, uint8_t * pt );

#endif
