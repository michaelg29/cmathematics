#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/util/numio.h"
#include "../cmathematics/data/hashing/sha.h"
#include "../cmathematics/data/hashing/hmac.h"
#include "../cmathematics/data/hashing/pbkdf.h"

int main()
{
    printf("Hello, world!\n");

    unsigned char *key = scanHex("0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b", 20);
    unsigned char *txt = scanHex("4869205468657265", 8);

    unsigned char *hmac_out = NULL;
    unsigned char *out = NULL;

    int len = hmac_sha(key, 20, txt, 8, SHA224_STR, &hmac_out);
    out = printByteArr(hmac_out, len, NULL, 0, 0);
    printf("%s\n", out);

    free(key);
    free(txt);
    free(out);
    free(hmac_out);

    key = "password";
    txt = "salt";
    unsigned char *dk = NULL;

    pbkdf2_hmac_sha(key, 8, txt, 4, 16777216, SHA256_STR, 20, &dk);
    out = printByteArr(dk, 20, 0, 0, 0);
    printf("%s\n", out);

    free(key);
    free(txt);
    free(dk);
    free(out);

    return 0;
}