#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <monocypher.h>

#include "encryption.h"


static const unsigned char base64_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_encode - Base64 encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out_len: Pointer to output length variable, or %NULL if not used
 * Returns: Allocated buffer of out_len bytes of encoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer. Returned buffer is
 * nul terminated to make it easier to use as a C string. The nul terminator is
 * not included in out_len.
 */
unsigned char * base64_encode(const unsigned char *src, size_t len,
			      size_t *out_len)
{
	unsigned char *out, *pos;
	const unsigned char *end, *in;
	size_t olen;
	int line_len;

	olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen += olen / 72; /* line feeds */
	olen++; /* nul termination */
	if (olen < len)
		return NULL; /* integer overflow */
	out = (unsigned char *)malloc(olen);
	if (out == NULL)
		return NULL;

	end = src + len;
	in = src;
	pos = out;
	line_len = 0;
	while (end - in >= 3) {
		*pos++ = base64_table[in[0] >> 2];
		*pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64_table[in[2] & 0x3f];
		in += 3;
		line_len += 4;
		if (line_len >= 72) {
			*pos++ = '\n';
			line_len = 0;
		}
	}

	if (end - in) {
		*pos++ = base64_table[in[0] >> 2];
		if (end - in == 1) {
			*pos++ = base64_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64_table[((in[0] & 0x03) << 4) |
					      (in[1] >> 4)];
			*pos++ = base64_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
		line_len += 4;
	}

	if (line_len)
		*pos++ = '\n';

	*pos = '\0';
	if (out_len)
		*out_len = pos - out;
	return out;
}


/**
 * base64_decode - Base64 decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
unsigned char * base64_decode(const unsigned char *src, size_t len,
			      size_t *out_len)
{
	unsigned char dtable[256], *out, *pos, block[4], tmp;
	size_t i, count, olen;
	int pad = 0;

	memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(base64_table) - 1; i++)
		dtable[base64_table[i]] = (unsigned char) i;
	dtable['='] = 0;

	count = 0;
	for (i = 0; i < len; i++) {
		if (dtable[src[i]] != 0x80)
			count++;
	}

	if (count == 0 || count % 4)
		return NULL;

	olen = count / 4 * 3;
	pos = out = (unsigned char *)malloc(olen);
	if (out == NULL)
		return NULL;

	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[src[i]];
		if (tmp == 0x80)
			continue;

		if (src[i] == '=')
			pad++;
		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)
					pos--;
				else if (pad == 2)
					pos -= 2;
				else {
					/* Invalid padding */
					free(out);
					return NULL;
				}
				break;
			}
		}
	}

	*out_len = pos - out;
	return out;
}

int gen_randon_nonce(EncryptedBytes *eb)
{
    if (!eb)
        return FAILURE;

    srand((unsigned int)time(NULL));
    unsigned char stream[NONCE_LEN];

    size_t i;

    for (i = 0; i < NONCE_LEN; i++)
    {
        stream[i] = rand();
    }

    memcpy(eb->nonce, stream, NONCE_LEN);
    return SUCCESS;
}

//todo srand errors and no checking
int gen_rdm_bytestream(LocalKeys *km, size_t num_bytes)
{
    if (!km)
        return FAILURE;
    srand((unsigned int)time(NULL));
    unsigned char *stream = (unsigned char *)malloc(num_bytes);
    size_t i;

    for (i = 0; i < num_bytes; i++)
    {
        stream[i] = rand();
    }

    memcpy(km->my_priv_key, stream, num_bytes);

    ///!_SKELETON
    ///!_SKELETON

    ///!_SOLUTION
    free(stream);
    ///!_SOLUTION
    return SUCCESS;
}
///!_SOLUTION

//if you are reading this what is wrong with this function
void eb_init(EncryptedBytes *eb)
{
    if (!eb)
        return;
    eb->cypher_text = NULL;
    eb->len = 0;
}

//and if you are reading this what is wrong with this function
void eb_free(EncryptedBytes *eb)
{
    if (!eb)
        return;
    free(eb->cypher_text);
    eb->len = 0;
}

//and if you are reading this what is wrong with this function
void db_init(DecryptedBytes *db)
{
    if (!db)
        return;
    db->plain_text = NULL;
    db->len = 0;
}

//and if you are reading this what is wrong with this function
void db_free(DecryptedBytes *db)
{
    if (!db)
        return;
    free(db->plain_text);
    db->len = 0;
}

void print_key(uint8_t*k,char * name){
	DEBUG_PRINT("%s key: ",name);
    for (int i = 0; i < KEY_LEN; i++) {
        printf("%02X", (unsigned char)k[i]);
    }
    printf("\n");
}

void print_nonce(uint8_t*k){
	DEBUG_PRINT("NONCE: ");    
	for (int i = 0; i < NONCE_LEN; i++) {
        printf("%02X", (unsigned char)k[i]);
    }
    printf("\n");
}

void print_mac(uint8_t*k){
	DEBUG_PRINT("MAC: ");
    for (int i = 0; i < MAC_LEN; i++) {
        printf("%02X", (unsigned char)k[i]);
    }
    printf("\n");
}

void print_bytes(uint8_t*k,int len){
	DEBUG_PRINT("BYTES: ");
    for (int i = 0; i < len; i++) {
        printf("%02X", (unsigned char)k[i]);
    }
    printf("\n");
}


int gen_keys(LocalKeys *km)
{
    if (!km)
        return FAILURE;
    gen_rdm_bytestream(km, KEY_LEN);

    crypto_key_exchange_public_key(km->my_pub_key, km->my_priv_key);
    return SUCCESS;
}

int derive_session_key(KeyMat *km)
{
    if (!km)
        return FAILURE;

    crypto_key_exchange(km->shared_key, km->my_priv_key, km->sender_pub_key);
    return SUCCESS;
}

int enc(EncryptedBytes *eb, KeyMat *km, DecryptedBytes *db)
{
    if (!eb || !km || !db || eb->cypher_text != NULL)
        return FAILURE;
    DEBUG_PRINT("encrypyting %u bytes...", db->len);

    eb->cypher_text = (uint8_t *)malloc(db->len);
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

int dec(DecryptedBytes *db, KeyMat *km, EncryptedBytes *eb)
{
    if (!eb || !km || !db)
        return FAILURE;
    DEBUG_PRINT("decrypting %u bytes...", eb->len);

    db->plain_text = (uint8_t *)malloc(eb->len);
    db->len = eb->len;
	print_nonce(eb->nonce);
	print_mac(eb->mac);
	print_bytes(eb->cypher_text,eb->len);
	
    crypto_unlock(db->plain_text, km->shared_key, eb->nonce, eb->mac, eb->cypher_text, eb->len);
	DEBUG_PRINT("decrypted %s", db->plain_text);
    return SUCCESS;
}