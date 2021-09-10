#include "sha1.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int sha1_h[5] = {
    0x67452301,
    0xEFCDAB89,
    0x98BADCFE,
    0x10325476,
    0xC3D2E1F0};

unsigned int sha1_k[4] = {
    0x5A827999,
    0x6ED9EBA1,
    0x8F1BBCDC,
    0xCA62C1D6};

unsigned int sha1_rotWord(unsigned int w, unsigned d)
{
    d &= 0x1f; // mod 32

    return (w << d) | (w >> (32 - d));
}

void sha1_initContext(sha1_context *ctx)
{
    ctx->msgLen = 0;
    memcpy(ctx->h, sha1_h, 5 * sizeof(unsigned int));
    ctx->stateCursor = 0;
}

void sha1_update(sha1_context *ctx, unsigned char *in, int n)
{
    int msgCursor = 0;

    while (msgCursor < n)
    {
        // determine if end of block or end of message comes first
        int noBytesInBlock = MIN(SHA1_BLOCK_LEN - ctx->stateCursor, n - msgCursor);

        // copy bytes
        memcpy(ctx->state + ctx->stateCursor, in + msgCursor, noBytesInBlock);

        // advance cursors
        msgCursor += noBytesInBlock;
        ctx->stateCursor += noBytesInBlock;

        if (ctx->stateCursor == SHA1_BLOCK_LEN)
        {
            // reached the end of the block

            // call the function
            sha1_f(ctx->h, ctx->state);

            // reset state
            ctx->stateCursor = 0;
        }
    }

    ctx->msgLen += n << 3; // length in bits
}

void sha1_digest(sha1_context *ctx, unsigned char **out)
{
    // PADDING

    // first bit 1
    ctx->state[ctx->stateCursor++] = 0x80;
    // rest of bits to 0
    memset(ctx->state + ctx->stateCursor, 0, MAX(SHA1_BLOCK_LEN - ctx->stateCursor, 0));

    // output size
    if (ctx->stateCursor >= (SHA1_BLOCK_LEN - sizeof(unsigned long long)))
    {
        // need new block to write message length

        // call function
        sha1_f(ctx->h, ctx->state);

        // reset state
        ctx->stateCursor = 0;
        memset(ctx->state, 0, SHA1_BLOCK_LEN);
    }
    // set last 64 bits as length of message
    unsigned long long size = ctx->msgLen;
    for (int i = (512 >> 3) - 1; size; i--)
    {
        // set LSByte on rightmost slot
        ctx->state[i] = size; // gets LSByte of long
        size >>= 8;           // remove LSByte
    }

    // call function on the last block
    sha1_f(ctx->h, ctx->state);

    // reset state
    ctx->stateCursor = 0;

    // output is the array ctx->h
    *out = malloc(SHA1_OUT * sizeof(unsigned char));
    if (!(*out))
    {
        // ensure memory was allocated
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 3; j >= 0; j--)
        {
            // get LSByte on right side
            (*out)[i * 4 + j] = ctx->h[i];
            ctx->h[i] >>= 8; // remove LSByte
        }
    }
}

void sha1_f(unsigned int h[5], unsigned char state[SHA1_BLOCK_LEN])
{
    // initialize W
    unsigned int W[16];
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            W[i] <<= 8;               // increase magnitude by 1 byte
            W[i] |= state[i * 4 + j]; // copy in byte
        }
    }

    // initialize function variables
    unsigned int A = h[0];
    unsigned int B = h[1];
    unsigned int C = h[2];
    unsigned int D = h[3];
    unsigned int E = h[4];

    // go through rounds
    for (int t = 0; t < SHA1_NR; t++)
    {
        int s = t & 0xf; // mod 15

        if (t >= 16)
        {
            // calculate new word
            W[s] = sha1_rotWord(
                W[(s + 13) & 0xf] ^ W[(s + 8) & 0xf] ^ W[(s + 2) & 0xf] ^ W[s],
                1);
        }

        // calculate temporary value
        unsigned int tmp = sha1_rotWord(A, 5) + E + W[s];
        // now add K_t + f_t(B, C, D)
        if (t < 20)
        {
            tmp += sha1_k[0] + ((B & C) | (~B & D));
        }
        else if (t < 40)
        {
            tmp += sha1_k[1] + (B ^ C ^ D);
        }
        else if (t < 60)
        {
            tmp += sha1_k[2] + ((B & C) | (B & D) | (C & D));
        }
        else
        {
            tmp += sha1_k[3] + (B ^ C ^ D);
        }

        E = D;
        D = C;
        C = sha1_rotWord(B, 30);
        B = A;
        A = tmp;
    }

    h[0] += A;
    h[1] += B;
    h[2] += C;
    h[3] += D;
    h[4] += E;
}