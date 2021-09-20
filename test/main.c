#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/data/hashing/sha1.h"
#include "../cmathematics/data/hashing/sha2.h"
#include "../cmathematics/data/hashing/sha3.h"

int main()
{
    printf("Hello, world!\n");

    unsigned char *msg = NULL;
    unsigned char *hash = NULL;
    unsigned char *out = NULL;

    // SHA-1
    msg = malloc(100 * sizeof(unsigned char));
    memset(msg, 'a', 100 * sizeof(unsigned char));
    sha1_context ctx1;
    sha1_initContext(&ctx1);
    for (int i = 0; i < 10000; i++)
    {
        sha1_update(&ctx1, msg, 100);
    }
    sha1_digest(&ctx1, &hash);

    out = printByteArr(hash, SHA1_OUT, NULL, 0, 0);
    printf("SHA1: %s\n", out);
    free(out);

    free(msg);
    free(hash);

    // SHA-224
    msg = "The quick brown fox jumps over the lazy dog";
    sha224_context ctx224;
    sha224_initContext(&ctx224);
    sha224_update(&ctx224, msg, 43);
    sha224_digest(&ctx224, &hash);

    out = printByteArr(hash, SHA224_OUT, NULL, 0, 0);
    printf("SHA224: %s\n", out);
    free(out);
    free(msg);
    free(hash);

    // SHA-256
    msg = "abc";
    sha256_context ctx256;
    sha256_initContext(&ctx256);
    sha256_update(&ctx256, msg, 3);
    sha256_digest(&ctx256, &hash);

    out = printByteArr(hash, SHA256_OUT, NULL, 0, 0);
    printf("SHA256: %s\n", out);
    free(out);
    free(msg);
    free(hash);

    // SHA-384
    msg = "";
    sha384_context ctx384;
    sha384_initContext(&ctx384);
    sha384_update(&ctx384, msg, 0);
    sha384_digest(&ctx384, &hash);

    out = printByteArr(hash, SHA384_OUT, NULL, 0, 0);
    printf("SHA384: %s\n", out);
    free(out);
    free(hash);

    // SHA-512
    sha512_context ctx512;
    sha512_initContext(&ctx512);
    sha512_update(&ctx512, msg, 0);
    sha512_digest(&ctx512, &hash);

    out = printByteArr(hash, SHA512_OUT, NULL, 0, 0);
    printf("SHA512: %s\n", out);
    free(msg);
    free(out);
    free(hash);

    // SHA-3
    int N = 176;
    msg = scanHex("1F42ADD25C0A80A4C82AAE3A0E302ABF9261DCA7E7884FD869D96ED4CE88AAAA25304D2D79E1FA5CC1FA2C95899229BC87431AD06DA524F2140E70BD0536E9685EE78089590A012384709182357091283479AB93489182CD7397F12B261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A", N);
    if (msg)
    {
        sha3_context ctx3;
        sha3_initContext(&ctx3, SHA3_256);
        sha3_update(&ctx3, msg, N);
        sha3_digest(&ctx3, &hash);
        out = printByteArr(hash, ctx3.ret_len, NULL, 0, 0);
        printf("SHA3-256: %s\n", out);
        free(out);
        free(hash);
    }
    else
    {
        printf("Invalid string\n");
    }
    free(msg);

    

    return 0;
}