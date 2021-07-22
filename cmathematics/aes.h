#include "cmathematics.h"

/*
 * Validation Tests:
 * https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/aesavs.pdf
 */

#ifndef AES_H
#define AES_H

#define AES_IRREDUCIBLE 0x1B

#define BLOCK_LEN 16
#define BLOCK_SIDE 4

#define AES_128 128
#define AES_192 192
#define AES_256 256

/*
    REFERENCE TABLES
*/

// substitution box
extern unsigned char aes_s_box[256];

// constant matrix for mix columns
extern unsigned char aes_mixColMat[BLOCK_SIDE][BLOCK_SIDE];

/*
    UTILITY METHODS
*/

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char galoisMul(unsigned char g1, unsigned char g2);

/*
    AES ENCRYPTION LAYERS
*/

void aes_addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]);
void aes_byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void aes_shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void aes_mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

void aes_encrypt_block(unsigned char *in_text, int n,
                       unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr,
                       unsigned char out[BLOCK_LEN]);

int aes_encrypt(unsigned char *in_text, int n,
                 unsigned char *in_key, int keylen,
                 unsigned char **out);

/*
    KEY SCHEDULING
*/

void aes_generateKeySchedule(unsigned char *in_key, int keylen, unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE]);
void aes_generateKeySchedule128(unsigned char *in_key, unsigned char subkeys[11][BLOCK_SIDE][BLOCK_SIDE]);

#endif