#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    REFERENCE TABLES
*/

// substitution box
unsigned char aes_s_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

// constant matrix for mix columns
unsigned char aes_mixColMat[BLOCK_SIDE][BLOCK_SIDE] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

/*
    UTILITY METHODS
*/

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char galoisMul(unsigned char g1, unsigned char g2)
{
    // taken and documented from https://en.wikipedia.org/wiki/Rijndael_MixColumns
    unsigned char p = 0;

    for (int i = 0; i < 8; i++)
    {
        if (g2 & 0x01) // if LSB is active (equivalent to a '1' in the polynomial of g2)
        {
            p ^= g1; // p += g1 in GF(2^8)
        }

        bool hiBit = (g1 & 0x80); // g1 >= 128 = 0100 0000
        g1 <<= 1;                 // rotate g1 left (multiply by x in GF(2^8))
        if (hiBit)
        {
            // must reduce
            g1 ^= AES_IRREDUCIBLE; // g1 -= 00011011 == mod(x^8 + x^4 + x^3 + x + 1) = AES irreducible
        }
        g2 >>= 1; // rotate g2 right (divide by x in GF(2^8))
    }

    return p;
}

/*
    AES ENCRYPTION LAYERS
*/

void aes_addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE])
{
    // add in GF(2^8) corresponding bytes of the subkey and state
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            state[r][c] ^= subkey[r][c];
        }
    }
}

void aes_byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    // substitute each byte using the s-box
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            state[r][c] = aes_s_box[state[r][c]];
        }
    }
}

void aes_shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    // rotate each row according to its position
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        leftRotate(state[r], r, BLOCK_SIDE);
    }
}

void aes_mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    unsigned char out[BLOCK_SIDE][BLOCK_SIDE];

    // matrix multiplication in GF(2^8)
    // * => galoisMul, + => ^
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            out[r][c] = 0x00;
            // dot product of the r-th row of mixColMat and the c-th col of state
            for (int i = 0; i < BLOCK_SIDE; i++)
            {
                out[r][c] ^= galoisMul(aes_mixColMat[r][i], state[i][c]);
            }
        }
    }

    // copy memory to the state
    memcpy(state, out, BLOCK_SIDE * BLOCK_SIDE * sizeof(unsigned char));
}

void aes_encrypt_block(unsigned char *in_text, int n,
                       unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr,
                       unsigned char out[BLOCK_LEN])
{
    // represent the state and key as a 4x4 table (read into columns)
    unsigned char state[BLOCK_SIDE][BLOCK_SIDE];
    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            state[r][c] = (i < n) ? in_text[i++] : 0;
        }
    }

    // ROUND 0
    aes_addRoundKey(state, subkeys[0]);

    // ROUNDS 1 --> NR-1
    for (i = 1; i < nr; i++)
    {
        aes_byteSub(state);
        aes_shiftRows(state);
        aes_mixCols(state);
        aes_addRoundKey(state, subkeys[i]);
    }

    // ROUND NR
    aes_byteSub(state);
    aes_shiftRows(state);
    aes_addRoundKey(state, subkeys[nr]);

    // copy bytes of state into the output column by column
    i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            out[i++] = state[r][c];
        }
    }
}

int aes_encrypt(unsigned char *in_text, int n,
                 unsigned char *in_key, int keylen,
                 unsigned char **out)
{
    // determine number of rounds
    int nr = 10; // AES_128 by default
    switch (keylen)
    {
    case AES_192:
        nr = 12;
        break;
    case AES_256:
        nr = 14;
        break;
    };

    // generate key schedule
    unsigned char(*subkeys)[BLOCK_SIDE][BLOCK_SIDE] =
        malloc((nr + 1) * sizeof(unsigned char[BLOCK_SIDE][BLOCK_SIDE]));
    aes_generateKeySchedule(in_key, keylen, subkeys);

    // divide input into blocks
    int noBlocks = n >> 4; // n / BLOCK_LEN
    int extra = n & 0x0f;  // n % BLOCK_LEN

    // allocate output
    int outLen = noBlocks * BLOCK_LEN;
    if (extra)
    {
        *out = malloc((noBlocks + 1) * BLOCK_LEN * sizeof(unsigned char));
        outLen += BLOCK_LEN;

        // encrypt extra text
        aes_encrypt_block(in_text + (noBlocks << 4), extra,
                          subkeys, nr,
                          *out + (noBlocks << 4));
    }
    else
    {
        *out = malloc(noBlocks * BLOCK_LEN * sizeof(unsigned char));
    }

    // encrypt complete blocks
    // i is the cursor position
    for (int i = 0; i < noBlocks; i++)
    {
        aes_encrypt_block(in_text + (i << 4), BLOCK_LEN, subkeys, nr, *out + (i << 4));
    }

    return outLen;
}

/*
    KEY SCHEDULING
*/

void aes_generateKeySchedule(unsigned char *in_key, int keylen, unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE])
{
    aes_generateKeySchedule128(in_key, subkeys);
}

void aes_generateKeySchedule128(unsigned char *in_key, unsigned char subkeys[11][BLOCK_SIDE][BLOCK_SIDE])
{
    // round 0 takes in the original key
    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            subkeys[0][r][c] = in_key[i++];
        }
    }

    // generate each round
    unsigned char roundCoeff = 0x01;
    /*
        Round 1: x^0 = 1
        Round 2: x^1 = x
        Round 3: x^2
        ...
        Round 9: x^8 def x^8 mod P(x)
    */
    for (i = 1; i <= 10; i++)
    {
        // transform key
        unsigned char g[4] = {
            aes_s_box[subkeys[i - 1][1][3]] ^ roundCoeff,
            aes_s_box[subkeys[i - 1][2][3]],
            aes_s_box[subkeys[i - 1][3][3]],
            aes_s_box[subkeys[i - 1][0][3]]};

        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            subkeys[i][r][0] = subkeys[i - 1][r][0] ^ g[r];
        }

        for (int c = 1; c < BLOCK_SIDE; c++)
        {
            for (int r = 0; r < BLOCK_SIDE; r++)
            {
                subkeys[i][r][c] = subkeys[i - 1][r][c] ^ subkeys[i][r][c - 1];
            }
        }

        // increment round coefficient
        roundCoeff = galoisMul(roundCoeff, 0x02); // multiply by x
    }
}