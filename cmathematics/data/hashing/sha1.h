#include "../../cmathematics.h"

#include "sha.h"

#ifndef SHA1_H
#define SHA1_H

#define SHA1_NR 80

// SHA1 constants
extern unsigned int sha1_h[5];
extern unsigned int sha1_k[4];

typedef struct sha1_context
{
    unsigned long long msgLen;

    // buffer for the output
    unsigned int h[5];

    // state values
    int stateCursor;
    unsigned char state[64];
} sha1_context;

void sha1_initContext(sha1_context *ctx);
void sha1_update(sha1_context *ctx, unsigned char *in, int n);
void sha1_digest(sha1_context *ctx, unsigned char **out);

void sha1_f(unsigned int h[5], unsigned char state[64]);

#endif // SHA1_H