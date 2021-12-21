#include "../../cmathematics.h"

#ifndef SHA_H
#define SHA_H

#define SHA1 0
#define SHA224 1
#define SHA256 2
#define SHA384 3
#define SHA512 4
#define SHA3_128 5
#define SHA3_256 6
#define SHA3_512 7

#define SHA1_STR "SHA-1"
#define SHA224_STR "SHA-224"
#define SHA256_STR "SHA-256"
#define SHA384_STR "SHA-384"
#define SHA512_STR "SHA-512"
#define SHA3_128_STR "SHA3-128"
#define SHA3_256_STR "SHA3-256"
#define SHA3_512_STR "SHA3-512"

extern int sha_blockLen[8];
extern int sha_retLen[8];

int sha_getModeNum(char *mode);

int sha_getBlockLen(char *mode);
int sha_getBlockLenIdx(int mode);
int sha_getRetLen(char *mode);
int sha_getRetLenIdx(int mode);

void *sha_initContextStr(char *mode);
void *sha_initContext(int mode);

void sha_updateStr(char *mode, void *ctx, unsigned char *in, int n);
void sha_update(int mode, void *ctx, unsigned char *in, int n);

void sha_digestStr(char *mode, void *ctx, unsigned char **out);
void sha_digest(int mode, void *ctx, unsigned char **out);

void sha_free(void *ctx);

#endif // SHA_H