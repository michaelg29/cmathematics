#include "sha.h"
#include "sha1.h"
#include "sha2.h"
#include "sha3.h"

#include <stdlib.h>
#include <string.h>

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

void sha_updateStr(char *mode, void *ctx, unsigned char *in, int n)
{
    sha_update(sha_getModeNum(mode), ctx, in, n);
}

void sha_update(int mode, void *ctx, unsigned char *in, int n) {
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

int sha_digestStr(char *mode, void *ctx, unsigned char **out)
{
    return sha_digest(sha_getModeNum(mode), ctx, out);
}

int sha_digest(int mode, void *ctx, unsigned char **out)
{
    switch (mode)
    {
        case SHA1:
            sha1_digest(ctx, out);
            return SHA1_OUT;
        case SHA224:
            sha224_digest(ctx, out);
            return SHA224_OUT;
        case SHA256:
            sha256_digest(ctx, out);
            return SHA256_OUT;
        case SHA384:
            sha384_digest(ctx, out);
            return SHA384_OUT;
        case SHA512:
            sha512_digest(ctx, out);
            return SHA512_OUT;
        case SHA3_128:
            sha3_digest(ctx, out);
            return ((sha3_context*)ctx)->ret_len;
        case SHA3_256:
            sha3_digest(ctx, out);
            return ((sha3_context*)ctx)->ret_len;
        default: // SHA3_512
            sha3_digest(ctx, out);
            return ((sha3_context*)ctx)->ret_len;
    };
}

void sha_free(void *ctx)
{
    free(ctx);
}