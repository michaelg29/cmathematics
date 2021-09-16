#include "../../cmathematics.h"

#ifndef SHA2_H
#define SHA2_H

#define SHA224 224
#define SHA256 256
#define SHA384 384
#define SHA512 512

#define SHA224256_BLOCK_LEN 64
#define SHA384512_BLOCK_LEN 128

#define SHA224_OUT SHA224 >> 3
#define SHA256_OUT SHA256 >> 3
#define SHA384_OUT SHA384 >> 3
#define SHA512_OUT SHA512 >> 3

#define SHA224256_NR 64
#define SHA384512_NR 80

extern unsigned int sha224_h[8];
extern unsigned int sha256_h[8];
extern unsigned int sha224256_k[64];
extern unsigned long long sha384_h[8];
extern unsigned long long sha512_h[8];
extern unsigned long long sha384512_k[80];

typedef struct sha224256_context
{
    int mode;
    unsigned long long msgLen;

    // buffer for the output
    unsigned int h[8];

    // state values
    int stateCursor;
    unsigned char state[SHA224256_BLOCK_LEN];
} sha224256_context;
typedef sha224256_context sha224_context;
typedef sha224256_context sha256_context;

void sha224_initContext(sha224_context *ctx);
void sha224_update(sha224_context *ctx, unsigned char *in, int n);
void sha224_digest(sha224_context *ctx, unsigned char **out);

void sha256_initContext(sha256_context *ctx);
void sha256_update(sha256_context *ctx, unsigned char *in, int n);
void sha256_digest(sha256_context *ctx, unsigned char **out);

void sha224256_update(sha224256_context *ctx, unsigned char *in, int n);
void sha224256_digest(sha224256_context *ctx, unsigned char **out, int outLen);
void sha224256_f(unsigned int h[8], unsigned char state[SHA224256_BLOCK_LEN]);

#endif