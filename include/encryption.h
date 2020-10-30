
#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <monocypher.h>
#include "datatypes.h"
#include "config.h"

extern "C" unsigned char * base64_encode(const unsigned char *src, size_t len,
			      size_t *out_len);
extern "C" unsigned char * base64_decode(const unsigned char *src, size_t len,
			      size_t *out_len);
extern "C" void db_init(DecryptedBytes *db);
extern "C" void db_free(DecryptedBytes *db);
extern "C" void eb_init(EncryptedBytes *eb);
extern "C" void eb_free(EncryptedBytes *eb);
extern "C" int gen_keys(LocalKeys *km);
extern "C" int derive_session_key(KeyMat *km);
extern "C" int enc(EncryptedBytes *eb, KeyMat *km, DecryptedBytes *db);
extern "C" int dec(DecryptedBytes *db, KeyMat *km, EncryptedBytes *eb);
extern "C" void print_key(uint8_t*k,char * name);
extern "C" void print_bytes(uint8_t*k,int len);
#endif
