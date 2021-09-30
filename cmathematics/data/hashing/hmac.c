#include "hmac.h"

#include "sha.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hmac_sha(unsigned char *key, int keyLen,
             unsigned char *txt, int txtLen,
             char *sha_mode,
             unsigned char **out)
{
    int mode = sha_getModeNum(sha_mode);
    int B = sha_getBlockLenIdx(mode);
    int L = sha_getRetLenIdx(mode);

    // allocate variables
    void *sha_ctx = NULL;
    unsigned char *outerKey = malloc(B + L);
    unsigned char *innerKey = malloc(B + txtLen);

    unsigned char *tmp = NULL;

    // generate hash key, K_0
    memset(outerKey, 0, B + L);
    memset(innerKey, 0, B + txtLen);
    if (keyLen > B)
    {
        // hash to get L bytes
        sha_ctx = sha_initContext(mode);
        sha_update(mode, sha_ctx, key, keyLen);
        sha_digest(mode, sha_ctx, &tmp);
        sha_free(sha_ctx);

        // copy memory to inner and outer keys
        memcpy(outerKey, tmp, MIN(B, L));
        memcpy(innerKey, tmp, MIN(B, L));

        free(tmp);
    }
    else
    {
        // copy key over
        memcpy(outerKey, key, keyLen);
        memcpy(innerKey, key, keyLen);
    }

    // XOR keys with respective bytes
    for (int i = 0; i < B; i++)
    {
        outerKey[i] ^= 0x5c; // opad byte
        innerKey[i] ^= 0x36; // ipad byte
    }

    // concatenate text to innerKey
    memcpy(innerKey + B, txt, txtLen);

    // hash inner key and concatenate to outer key
    sha_ctx = sha_initContext(mode);
    sha_update(mode, sha_ctx, innerKey, B + txtLen);
    sha_digest(mode, sha_ctx, &tmp);
    sha_free(sha_ctx);
    memcpy(outerKey + B, tmp, L);
    free(tmp);

    // hash outerKey
    sha_ctx = sha_initContext(mode);
    sha_update(mode, sha_ctx, outerKey, B + L);
    sha_digest(mode, sha_ctx, out);
    sha_free(sha_ctx);

    free(outerKey);
    free(innerKey);

    return L;
}