#include "../../cmathematics.h"

#include "sha.h"

#ifndef SHA3_H
#define SHA3_H

#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 << SHA3_L)

extern unsigned int sha3_rotConst[5][5];
extern unsigned long long sha3_roundConsts[SHA3_NR];

typedef struct sha3_context
{
    int ret_len; // hash output size
    int r; // bit rate

    int stateCursor; // cursor (bytes) in the state array
    unsigned long long A[5][5]; // state array
} sha3_context;

void sha3_initContext(sha3_context *ctx, int mode);
void sha3_update(sha3_context *ctx, unsigned char *in, int n);
void sha3_digest(sha3_context *ctx, unsigned char **out);

void sha3_keccak_f(unsigned long long A[5][5]);

#endif // SHA3_H