#include "../../cmathematics.h"

#ifndef SHA3_H
#define SHA3_H

#define SHA3_L 6
#define SHA3_NR (12 + 2 * SHA3_L)
#define SHA3_B (25 << SHA3_L)

#define SHA3_128 128
#define SHA3_256 256
#define SHA3_512 512

extern unsigned int sha3_rotConst[5][5];
extern unsigned long long sha3_roundConsts[SHA3_NR];

void sha3_keccak_f(unsigned long long A[5][5]);
int sha3_hash(unsigned char *in, int n, int mode, unsigned char **out);

#endif