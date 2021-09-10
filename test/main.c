#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/sha3.h"
#include "../cmathematics/sha1.h"

int main()
{
    printf("Hello, world!\n");

    unsigned char *msg = NULL;
    unsigned char *hash = NULL;
    unsigned char *out = NULL;

    const int N = 176;
    msg = scanHex("1F42ADD25C0A80A4C82AAE3A0E302ABF9261DCA7E7884FD869D96ED4CE88AAAA25304D2D79E1FA5CC1FA2C95899229BC87431AD06DA524F2140E70BD0536E9685EE78089590A012384709182357091283479AB93489182CD7397F12B261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A", N);
    if (msg)
    {
        int len = sha3_hash(msg, 176, SHA3_256, &hash);
        printf("%d\n", len);
        out = printByteArr(hash, len, NULL, 0, 0);
        printf("%s\n", out);
        free(out);
        free(hash);
    }
    else
    {
        printf("Invalid string\n");
    }
    free(msg);

    msg = malloc(100 * sizeof(unsigned char));
    memset(msg, 'a', 100 * sizeof(unsigned char));
    sha1_context ctx;
    sha1_initContext(&ctx);
    for (int i = 0; i < 10000; i++)
    {
        sha1_update(&ctx, msg, 100);
    }
    sha1_digest(&ctx, &hash);

    out = printByteArr(hash, SHA1_OUT, " ", 1, 2);
    printf("%s\n", out);
    free(out);

    free(msg);
    free(hash);

    return 0;
}