#include "../../cmathematics.h"

/*
 * Validation Tests:
 * https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/aesavs.pdf
 */

#ifndef AES_H
#define AES_H

#define AES_IRREDUCIBLE 0x1B

#define AES_BLOCK_LEN 16
#define AES_BLOCK_SIDE 4

// AES key lengths
#define AES_128 128
#define AES_192 192
#define AES_256 256

// number of rounds based on the key length
#define AES_128_NR 10
#define AES_192_NR 12
#define AES_256_NR 14

// AES modes
#define AES_ECB 0
#define AES_CBC 1
#define AES_CTR 2

/*
    REFERENCE TABLES
*/

// substitution box
extern unsigned char aes_s_box[256];
extern unsigned char aes_inv_s_box[256];

// constant matrix for mix columns
extern unsigned char aes_mixColMat[AES_BLOCK_SIDE][AES_BLOCK_SIDE];
extern unsigned char aes_inv_mixColMat[AES_BLOCK_SIDE][AES_BLOCK_SIDE];

/*
    UTILITY METHODS
*/

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char galoisMul(unsigned char g1, unsigned char g2);

/*
    AES ENCRYPTION LAYERS
*/

void aes_addRoundKey(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE], unsigned char subkey[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_byteSub(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_shiftRows(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_mixCols(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);

void aes_encrypt_block(unsigned char *in_text, int n,
                       unsigned char subkeys[][AES_BLOCK_SIDE][AES_BLOCK_SIDE], int nr,
                       unsigned char iv[AES_BLOCK_LEN],
                       unsigned char out[AES_BLOCK_LEN]);

int aes_encrypt_withSchedule(unsigned char *in_text, int n,
                             unsigned char subkeys[][AES_BLOCK_SIDE][AES_BLOCK_SIDE], int nr,
                             unsigned char mode,
                             unsigned char iv[AES_BLOCK_LEN],
                             unsigned char **out);

int aes_encrypt(unsigned char *in_text, int n,
                unsigned char *in_key, int keylen,
                unsigned char mode,
                unsigned char iv[AES_BLOCK_LEN],
                unsigned char **out);

/*
    AES DECRYPTION LAYERS
*/

void aes_inv_addRoundKey(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE], unsigned char subkey[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_inv_byteSub(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_inv_shiftRows(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_inv_mixCols(unsigned char state[AES_BLOCK_SIDE][AES_BLOCK_SIDE]);

void aes_decrypt_block(unsigned char *in_cipher,
                       unsigned char subkeys[][AES_BLOCK_SIDE][AES_BLOCK_SIDE], int nr,
                       unsigned char iv[AES_BLOCK_LEN],
                       unsigned char out[AES_BLOCK_LEN]);

int aes_decrypt_withSchedule(unsigned char *in_cipher, int n,
                             unsigned char subkeys[][AES_BLOCK_SIDE][AES_BLOCK_SIDE], int nr,
                             unsigned char mode,
                             unsigned char iv[AES_BLOCK_LEN],
                             unsigned char **out);

int aes_decrypt(unsigned char *in_cipher, int n,
                unsigned char *in_key, int keylen,
                unsigned char mode,
                unsigned char iv[AES_BLOCK_LEN],
                unsigned char **out);

/*
    KEY SCHEDULING
*/

void aes_generateKeySchedule(unsigned char *in_key, int keylen, unsigned char subkeys[][AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_generateKeySchedule128(unsigned char *in_key, unsigned char subkeys[AES_128_NR + 1][AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_generateKeySchedule192(unsigned char *in_key, unsigned char subkeys[AES_192_NR + 1][AES_BLOCK_SIDE][AES_BLOCK_SIDE]);
void aes_generateKeySchedule256(unsigned char *in_key, unsigned char subkeys[AES_256_NR + 1][AES_BLOCK_SIDE][AES_BLOCK_SIDE]);

void aes_incrementCounter(unsigned char iv[AES_BLOCK_LEN], unsigned int inc);

#endif // AES_H