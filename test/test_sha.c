#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/util/numio.h"
#include "../cmathematics/data/hashing/sha.h"
#include "../cmathematics/data/hashing/sha1.h"
#include "../cmathematics/data/hashing/sha2.h"
#include "../cmathematics/data/hashing/sha3.h"

int main()
{
    printf("Hello, world!\n");

    unsigned char *msg = NULL;
    unsigned char *hash = NULL;
    unsigned char *out = NULL;

    char *modes[8] = {
        SHA1_STR,
        SHA224_STR,
        SHA256_STR,
        SHA384_STR,
        SHA512_STR,
        SHA3_128_STR,
        SHA3_256_STR,
        SHA3_512_STR
    };

    msg = "abcdefghi";

    for (int i = 0; i < 8; i++) {
        void *ctx = sha_initContextStr(modes[i]);
        sha_updateStr(modes[i], ctx, msg, 9);
        sha_updateStr(modes[i], ctx, msg + 2, 7);
        sha_digestStr(modes[i], ctx, &hash);
        sha_free(ctx);

        out = printByteArr(hash, sha_retLen[i], NULL, 0, 0);
        printf("%8s: %s\n", modes[i], out);
        free(out);
        free(hash);
    }

    free(msg);

    return 0;
}