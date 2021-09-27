#include "sha.h"
#include "sha1.h"
#include "sha2.h"
#include "sha3.h"

#include <stdlib.h>
#include <string.h>

int sha_blockLen[8] = {
    64,
    64,
    64,
    128,
    128,
    1344 >> 3,
    1088 >> 3,
    576 >> 3
};

int sha_retLen[8] = {
    160 >> 3,
    224 >> 3,
    256 >> 3,
    384 >> 3,
    512 >> 3,
    128 >> 3,
    256 >> 3,
    512 >> 3
};

int sha_getModeNum(char *mode)
{
    if (!strcmp(mode, SHA1_STR)) {
        return SHA1;
    }
    if (!strcmp(mode, SHA224_STR)) {
        return SHA224;
    }
    if (!strcmp(mode, SHA256_STR)) {
        return SHA256;
    }
    if (!strcmp(mode, SHA384_STR)) {
        return SHA384;
    }
    if (!strcmp(mode, SHA512_STR)) {
        return SHA512;
    }
    if (!strcmp(mode, SHA3_128_STR)) {
        return SHA3_128;
    }
    if (!strcmp(mode, SHA3_256_STR)) {
        return SHA3_256;
    }
    else { // default SHA3_512
        return SHA3_512;
    }
}

int sha_getBlockLen(char *mode)
{
    return sha_blockLen[sha_getModeNum(mode)];
}

int sha_getBlockLenIdx(int mode)
{
    if (mode < SHA1 || mode > SHA3_512)
    {
        mode = SHA3_512;
    }

    return sha_blockLen[mode];
}

int sha_getRetLen(char *mode)
{
    return sha_retLen[sha_getModeNum(mode)];
}

int sha_getRetLenIdx(int mode)
{
    if (mode < SHA1 || mode > SHA3_512)
    {
        mode = SHA3_512;
    }

    return sha_retLen[mode];
}

void *sha_initContextStr(char *mode)
{
    return sha_initContext(sha_getModeNum(mode));
}

void *sha_initContext(int mode)
{
    void *ctx = NULL;
    switch (mode)
    {
        case SHA1:
            ctx = malloc(sizeof(sha1_context));
            sha1_initContext(ctx);
            return ctx;
        case SHA224:
            ctx = malloc(sizeof(sha224_context));
            sha224_initContext(ctx);
            return ctx;
        case SHA256:
            ctx = malloc(sizeof(sha256_context));
            sha256_initContext(ctx);
            return ctx;
        case SHA384:
            ctx = malloc(sizeof(sha384_context));
            sha384_initContext(ctx);
            return ctx;
        case SHA512:
            ctx = malloc(sizeof(sha512_context));
            sha512_initContext(ctx);
            return ctx;
        case SHA3_128:
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(ctx, SHA3_128);
            return ctx;
        case SHA3_256:
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(ctx, SHA3_256);
            return ctx;
        default: // SHA3_512
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(ctx, SHA3_512);
            return ctx;
    };
}

#include <stdio.h>
void sha_updateStr(char *mode, void *ctx, unsigned char *in, int n)
{
    //printf("%s\n", mode);
    int i = sha_getModeNum(mode);
    //printf("%d\n", i);
    sha_update(i, ctx, in, n);
}

void sha_update(int mode, void *ctx, unsigned char *in, int n) {
    // char *test = malloc(n + 1);
    // memcpy(test, in, n);
    // test[n] = '\0';
    // printf("%s\n", test);

    switch (mode)
    {
        case SHA1:
            sha1_update(ctx, in, n);
            break;
        case SHA224:
            sha224_update(ctx, in, n);
            break;
        case SHA256:
            sha256_update(ctx, in, n);
            break;
        case SHA384:
            sha384_update(ctx, in, n);
            break;
        case SHA512:
            sha512_update(ctx, in, n);
            break;
        case SHA3_128:
            sha3_update(ctx, in, n);
            break;
        case SHA3_256:
            sha3_update(ctx, in, n);
            break;
        default: // SHA3_512
            sha3_update(ctx, in, n);
            break;
    };
}

void sha_digestStr(char *mode, void *ctx, unsigned char **out)
{
    sha_digest(sha_getModeNum(mode), ctx, out);
}

void sha_digest(int mode, void *ctx, unsigned char **out)
{
    switch (mode)
    {
        case SHA1:
            sha1_digest(ctx, out);
            return;
        case SHA224:
            sha224_digest(ctx, out);
            return;
        case SHA256:
            sha256_digest(ctx, out);
            return;
        case SHA384:
            sha384_digest(ctx, out);
            return;
        case SHA512:
            sha512_digest(ctx, out);
            return;
        case SHA3_128:
            sha3_digest(ctx, out);
            return;
        case SHA3_256:
            sha3_digest(ctx, out);
            return;
        default: // SHA3_512
            sha3_digest(ctx, out);
            return;
    };
}

void sha_free(void *ctx)
{
    free(ctx);
}