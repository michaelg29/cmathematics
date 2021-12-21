#include "sha.h"
#include "sha2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../lib/arrays.h"

unsigned int sha224_h[8] = {
    0xc1059ed8UL,
    0x367cd507UL,
    0x3070dd17UL,
    0xf70e5939UL,
    0xffc00b31UL,
    0x68581511UL,
    0x64f98fa7UL,
    0xbefa4fa4UL};

unsigned int sha256_h[8] = {
    0x6a09e667UL,
    0xbb67ae85UL,
    0x3c6ef372UL,
    0xa54ff53aUL,
    0x510e527fUL,
    0x9b05688cUL,
    0x1f83d9abUL,
    0x5be0cd19UL};

unsigned int sha224256_k[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL, 0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL};

unsigned long long sha384_h[8] = {
    0xcbbb9d5dc1059ed8ULL,
    0x629a292a367cd507ULL,
    0x9159015a3070dd17ULL,
    0x152fecd8f70e5939ULL,
    0x67332667ffc00b31ULL,
    0x8eb44a8768581511ULL,
    0xdb0c2e0d64f98fa7ULL,
    0x47b5481dbefa4fa4ULL};

unsigned long long sha512_h[8] = {
    0x6a09e667f3bcc908ULL,
    0xbb67ae8584caa73bULL,
    0x3c6ef372fe94f82bULL,
    0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL,
    0x9b05688c2b3e6c1fULL,
    0x1f83d9abfb41bd6bULL,
    0x5be0cd19137e2179ULL};

unsigned long long sha384512_k[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
    0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
    0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
    0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
    0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
    0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
    0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
    0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
    0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
    0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
    0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
    0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};

void sha224_initContext(sha224_context *ctx)
{
    ctx->mode = SHA224;
    ctx->msgLen = 0ULL;
    memcpy(ctx->h, sha224_h, 8 * sizeof(unsigned int));
    ctx->stateCursor = 0;
}

void sha224_update(sha224_context *ctx, unsigned char *in, int n)
{
    sha224256_update(ctx, in, n);
}

void sha224_digest(sha224_context *ctx, unsigned char **out)
{
    sha224256_digest(ctx, out, sha_retLen[SHA224]);
}

void sha256_initContext(sha256_context *ctx)
{
    ctx->mode = SHA256;
    ctx->msgLen = 0ULL;
    memcpy(ctx->h, sha256_h, 8 * sizeof(unsigned int));
    ctx->stateCursor = 0;
}

void sha256_update(sha256_context *ctx, unsigned char *in, int n)
{
    sha224256_update(ctx, in, n);
}

void sha256_digest(sha256_context *ctx, unsigned char **out)
{
    sha224256_digest(ctx, out, sha_retLen[SHA256]);
}

void sha224256_update(sha224256_context *ctx, unsigned char *in, int n)
{
    int msgCursor = 0;

    while (msgCursor < n)
    {
        // determine if end of block or end of message comes first
        int noBytesInBlock = MIN(sha_blockLen[SHA256] - ctx->stateCursor, n - msgCursor);

        // copy bytes
        memcpy(ctx->state + ctx->stateCursor, in + msgCursor, noBytesInBlock);

        // advance cursors
        msgCursor += noBytesInBlock;
        ctx->stateCursor += noBytesInBlock;

        if (ctx->stateCursor == sha_blockLen[SHA256])
        {
            // reached end of the block

            // call the function
            sha224256_f(ctx->h, ctx->state);

            // reset state
            ctx->stateCursor = 0;
        }
    }

    ctx->msgLen += n << 3; // length in bits
}

void sha224256_digest(sha224256_context *ctx, unsigned char **out, int outLen)
{
    // PADDING

    // first bit 1
    ctx->state[ctx->stateCursor++] = 0x80;
    // rest of bits to 0
    memset(ctx->state + ctx->stateCursor, 0, MAX(sha_blockLen[SHA256] - ctx->stateCursor, 0));

    // output size
    if (ctx->stateCursor >= (sha_blockLen[SHA256] - sizeof(unsigned long long)))
    {
        // need new block to write message length

        // call function on complete block
        sha224256_f(ctx->h, ctx->state);

        // reset state
        ctx->stateCursor = 0;
        memset(ctx->state, 0, sha_blockLen[SHA256]);
    }
    // set last 64 bits as length
    unsigned long long size = ctx->msgLen;
    for (int i = sha_blockLen[SHA256] - 1; size; i--)
    {
        // set LSByte on rightmost slot
        ctx->state[i] = size; // gets LSByte of long long
        size >>= 8;           // remove LSByte
    }

    // call function on the last block
    sha224256_f(ctx->h, ctx->state);

    // reset state
    ctx->stateCursor = 0;

    // output is the array ctx->h
    *out = malloc(outLen * sizeof(unsigned char));
    if (!(*out))
    {
        // ensure memory was allocated
        return;
    }

    int noWords = outLen / sizeof(unsigned int);
    for (int i = 0; i < noWords; i++)
    {
        for (int j = sizeof(unsigned int) - 1; j >= 0; j--)
        {
            // get LSByte on right side
            (*out)[i * sizeof(unsigned int) + j] = ctx->h[i];
            ctx->h[i] >>= 8; // remove LSByte
        }
    }
}

void sha224256_f(unsigned int h[8], unsigned char state[64])
{
    // initialize W
    unsigned int W[16];
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < sizeof(unsigned int); j++)
        {
            W[i] <<= 8;               // increase magnitude by 1 byte
            W[i] |= state[i * sizeof(unsigned int) + j]; // copy in byte
        }
        //printf("W[%2d] = %08lx\n", i, W[i]);
    }

    // initialize working variables
    unsigned int h2[8];
    memcpy(h2, h, 8 * sizeof(unsigned int));

    // go through rounds
    for (int t = 0; t < SHA224256_NR; t++)
    {
        int s = t & 0xf; // mod 16

        if (t >= 16)
        {
            // calculate new word
            unsigned int val_s0 = W[(s - 15) & 0xf];
            unsigned int val_s1 = W[(s - 2) & 0xf];

            unsigned int s0 = rightRotateI(val_s0, 7) ^ rightRotateI(val_s0, 18) ^ (val_s0 >> 3);
            unsigned int s1 = rightRotateI(val_s1, 17) ^ rightRotateI(val_s1, 19) ^ (val_s1 >> 10);

            // sigma_1(W_t-2) + W_t-7 + sigma_0(W_t-15) + W_t-16
            W[s] = W[s] + s0 + W[(s - 7) & 0xf] + s1;
        }

        // t1 = h + sum_1(e) + Ch(e, f, g) + K_t + W_t
		// t2 = sum_0(a) + Maj(a, b, c)
        unsigned int S1 = rightRotateI(h2[4], 6) ^ rightRotateI(h2[4], 11) ^ rightRotateI(h2[4], 25);
        unsigned int ch = (h2[4] & h2[5]) ^ (~h2[4] & h2[6]);
        unsigned int tmp1 = h2[7] + S1 + ch + sha224256_k[t] + W[s];
        unsigned int S0 = rightRotateI(h2[0], 2) ^ rightRotateI(h2[0], 13) ^ rightRotateI(h2[0], 22);
        unsigned int maj = (h2[0] & h2[1]) ^ (h2[0] & h2[2]) ^ (h2[1] & h2[2]);
        unsigned int tmp2 = S0 + maj;

        for (int i = 7; i > 0; i--)
        {
            h2[i] = h2[i - 1];
        }
        h2[4] += tmp1;
        h2[0] = tmp1 + tmp2;
    }

    // update buffer values
    for (int i = 0; i < 8; i++)
    {
        h[i] += h2[i];
    }
}

void sha384_initContext(sha384_context *ctx)
{
    ctx->mode = SHA384;
    ctx->msgLen[0] = 0ULL;
    ctx->msgLen[1] = 0ULL;
    memcpy(ctx->h, sha384_h, 8 * sizeof(unsigned long long));
    ctx->stateCursor = 0;
}

void sha384_update(sha384_context *ctx, unsigned char *in, int n)
{
    sha384512_update(ctx, in, n);
}

void sha384_digest(sha384_context *ctx, unsigned char **out)
{
    sha384512_digest(ctx, out, sha_retLen[SHA384]);
}

void sha512_initContext(sha512_context *ctx)
{
    ctx->mode = SHA512;
    ctx->msgLen[0] = 0ULL;
    ctx->msgLen[1] = 0ULL;
    memcpy(ctx->h, sha512_h, 8 * sizeof(unsigned long long));
    ctx->stateCursor = 0;
}

void sha512_update(sha512_context *ctx, unsigned char *in, int n)
{
    sha384512_update(ctx, in, n);
}

void sha512_digest(sha512_context *ctx, unsigned char **out)
{
    sha384512_digest(ctx, out, sha_retLen[SHA512]);
}

void sha384512_update(sha384512_context *ctx, unsigned char *in, int n)
{
    int msgCursor = 0;

    while (msgCursor < n)
    {
        // determine if end of block or end of message comes first
        int noBytesInBlock = MIN(sha_blockLen[SHA512] - ctx->stateCursor, n - msgCursor);

        // copy bytes
        memcpy(ctx->state + ctx->stateCursor, in + msgCursor, noBytesInBlock);

        // advance cursors
        msgCursor += noBytesInBlock;
        ctx->stateCursor += noBytesInBlock;

        if (ctx->stateCursor == sha_blockLen[SHA512])
        {
            // reached end of the block

            // call the function
            sha384512_f(ctx->h, ctx->state);

            // reset state
            ctx->stateCursor = 0;
        }
    }

    // add length in bits
    unsigned long long carry = n << 3; // => bits
    unsigned long long nextCarry = 0ULL; // => extra
    for (int i = 0; i < 2; i++)
    {
        if (carry)
        {
            unsigned long long initial = ctx->msgLen[i];
            ctx->msgLen[i] += carry;
            carry = 0ULL;
            if (ctx->msgLen[i] < initial)
            {
                carry = 1ULL;
            }
        }

        carry += nextCarry;
        nextCarry = 0ULL;
    }
}

void sha384512_digest(sha384512_context *ctx, unsigned char **out, int outLen)
{
    // PADDING

    // first bit 1
    ctx->state[ctx->stateCursor++] = 0x80;
    // rest of bits to 0
    memset(ctx->state + ctx->stateCursor, 0, MAX(sha_blockLen[SHA512] - ctx->stateCursor, 0));

    // output size
    if (ctx->stateCursor >= (sha_blockLen[SHA512] - 2 * sizeof(unsigned long long)))
    {
        // need new block to write message length

        // call function on complete block
        sha384512_f(ctx->h, ctx->state);

        // reset state
        ctx->stateCursor = 0;
        memset(ctx->state, 0, sha_blockLen[SHA512]);
    }
    // set last 128 bits as length
    unsigned long long size[2] = { ctx->msgLen[0], ctx->msgLen[1] };
    for (int i = sha_blockLen[SHA512] - 1, sizeIdx = 0, byteCounter = 0; byteCounter < 16; i--, byteCounter++)
    {
        // set LSByte on rightmost slot
        ctx->state[i] = size[sizeIdx]; // gets LSByte of long long
        size[sizeIdx] >>= 8;           // remove LSByte
        if (i == sha_blockLen[SHA512] - sizeof(unsigned long long))
        {
            sizeIdx++;
        }
    }

    // call function on the last block
    sha384512_f(ctx->h, ctx->state);

    // reset state
    ctx->stateCursor = 0;

    // output is the array ctx->h
    *out = malloc(outLen * sizeof(unsigned char));
    if (!(*out))
    {
        // ensure memory was allocated
        return;
    }

    int noWords = outLen / sizeof(unsigned long long);
    for (int i = 0; i < noWords; i++)
    {
        for (int j = sizeof(unsigned long long) - 1; j >= 0; j--)
        {
            // get LSByte on right side
            (*out)[i * sizeof(unsigned long long) + j] = ctx->h[i];
            ctx->h[i] >>= 8; // remove LSByte
        }
    }
}

void sha384512_f(unsigned long long h[8], unsigned char state[128])
{
    // initialize W
    unsigned long long W[16];
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < sizeof(unsigned long long); j++)
        {
            W[i] <<= 8;               // increase magnitude by 1 byte
            W[i] |= state[i * sizeof(unsigned long long) + j]; // copy in byte
        }
    }

    // initialize working variables
    unsigned long long h2[8];
    memcpy(h2, h, 8 * sizeof(unsigned long long));

    // go through rounds
    for (int t = 0; t < SHA384512_NR; t++)
    {
        int s = t & 0xf; // mod 16

        if (t >= 16)
        {
            // calculate new word
            unsigned long long val_s0 = W[(s - 15) & 0xf];
            unsigned long long val_s1 = W[(s - 2) & 0xf];

            unsigned long long s0 = rightRotateLL(val_s0, 1) ^ rightRotateLL(val_s0, 8) ^ (val_s0 >> 7);
            unsigned long long s1 = rightRotateLL(val_s1, 19) ^ rightRotateLL(val_s1, 61) ^ (val_s1 >> 6);

            // sigma_1(W_t-2) + W_t-7 + sigma_0(W_t-15) + W_t-16
            W[s] = W[s] + s0 + W[(s - 7) & 0xf] + s1;
        }

        // t1 = h + sum_1(e) + Ch(e, f, g) + K_t + W_t
		// t2 = sum_0(a) + Maj(a, b, c)
        unsigned long long S1 = rightRotateLL(h2[4], 14) ^ rightRotateLL(h2[4], 18) ^ rightRotateLL(h2[4], 41);
        unsigned long long ch = (h2[4] & h2[5]) ^ (~h2[4] & h2[6]);
        unsigned long long tmp1 = h2[7] + S1 + ch + sha384512_k[t] + W[s];
        unsigned long long S0 = rightRotateLL(h2[0], 28) ^ rightRotateLL(h2[0], 34) ^ rightRotateLL(h2[0], 39);
        unsigned long long maj = (h2[0] & h2[1]) ^ (h2[0] & h2[2]) ^ (h2[1] & h2[2]);
        unsigned long long tmp2 = S0 + maj;

        for (int i = 7; i > 0; i--)
        {
            h2[i] = h2[i - 1];
        }
        h2[4] += tmp1;
        h2[0] = tmp1 + tmp2;
    }

    // update buffer values
    for (int i = 0; i < 8; i++)
    {
        h[i] += h2[i];
    }
}