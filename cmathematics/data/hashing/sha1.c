#include "sha.h"
#include "sha1.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/arrays.h"

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

void sha1_initContext(sha1_context *ctx)
{
    ctx->msgLen = 0ULL;
    memcpy(ctx->h, sha1_h, 5 * sizeof(unsigned int));
    ctx->stateCursor = 0;
}

void sha1_update(sha1_context *ctx, unsigned char *in, int n)
{
    int msgCursor = 0;

    while (msgCursor < n)
    {
        // determine if end of block or end of message comes first
        int noBytesInBlock = MIN(sha_blockLen[SHA1] - ctx->stateCursor, n - msgCursor);

        // copy bytes
        memcpy(ctx->state + ctx->stateCursor, in + msgCursor, noBytesInBlock);

        // advance cursors
        msgCursor += noBytesInBlock;
        ctx->stateCursor += noBytesInBlock;

        if (ctx->stateCursor == sha_blockLen[SHA1])
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
    memset(ctx->state + ctx->stateCursor, 0, MAX(sha_blockLen[SHA1] - ctx->stateCursor, 0));

    // output size
    if (ctx->stateCursor >= (sha_blockLen[SHA1] - sizeof(unsigned long long)))
    {
        // need new block to write message length

        // call function
        sha1_f(ctx->h, ctx->state);

        // reset state
        ctx->stateCursor = 0;
        memset(ctx->state, 0, sha_blockLen[SHA1]);
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
    *out = malloc(sha_retLen[SHA1] * sizeof(unsigned char));
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

void sha1_f(unsigned int h[5], unsigned char state[64])
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
    unsigned int h2[5];
    for (int i = 0; i < 5; i++) {
        h2[i] = h[i];
    }

    // go through rounds
    for (int t = 0; t < SHA1_NR; t++)
    {
        int s = t & 0xf; // mod 15

        if (t >= 16)
        {
            // calculate new word
            W[s] = leftRotateI(
                W[(s + 13) & 0xf] ^ W[(s + 8) & 0xf] ^ W[(s + 2) & 0xf] ^ W[s],
                1);
        }

        // calculate temporary value
        unsigned int tmp = leftRotateI(h[0], 5) + h[4] + W[s];
        // now add K_t + f_t(h[1], h[2], h[3])
        if (t < 20)
        {
            tmp += sha1_k[0] + ((h[1] & h[2]) | (~h[1] & h[3]));
        }
        else if (t < 40)
        {
            tmp += sha1_k[1] + (h[1] ^ h[2] ^ h[3]);
        }
        else if (t < 60)
        {
            tmp += sha1_k[2] + ((h[1] & h[2]) | (h[1] & h[3]) | (h[2] & h[3]));
        }
        else
        {
            tmp += sha1_k[3] + (h[1] ^ h[2] ^ h[3]);
        }

        h[4] = h[3];
        h[3] = h[2];
        h[2] = leftRotateI(h[1], 30);
        h[1] = h[0];
        h[0] = tmp;
    }

    for (int i = 0; i < 5; i++) {
        h[i] += h2[i];
    }
}