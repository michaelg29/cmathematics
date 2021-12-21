#include "../../cmathematics.h"

#include "sha.h"

#ifndef SHA2_H
#define SHA2_H

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
    unsigned char state[64];
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
void sha224256_f(unsigned int h[8], unsigned char state[64]);

typedef struct sha384512_context
{
    int mode;
    unsigned long long msgLen[2];

    // buffer for the output
    unsigned long long h[8];

    // state values
    int stateCursor;
    unsigned char state[128];
} sha384512_context;
typedef sha384512_context sha384_context;
typedef sha384512_context sha512_context;

void sha384_initContext(sha384_context *ctx);
void sha384_update(sha384_context *ctx, unsigned char *in, int n);
void sha384_digest(sha384_context *ctx, unsigned char **out);

void sha512_initContext(sha512_context *ctx);
void sha512_update(sha512_context *ctx, unsigned char *in, int n);
void sha512_digest(sha512_context *ctx, unsigned char **out);

void sha384512_update(sha384512_context *ctx, unsigned char *in, int n);
void sha384512_digest(sha384512_context *ctx, unsigned char **out, int outLen);
void sha384512_f(unsigned long long h[8], unsigned char state[128]);

#endif // SHA2_H