#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/sha3.h"
#include "../cmathematics/sha1.h"

char hex[16] = "0123456789ABCDEF";
void printCharArr(unsigned char *arr, int len, bool hasSpace)
{
    printf("{ ");
    for (int i = 0; i < len; i++)
    {
        printf("%c%c%s", hex[arr[i] >> 4], hex[arr[i] & 0x0f], hasSpace ? " " : "");
    }
    printf("%s}\n", hasSpace ? "" : " ");
}

unsigned char *scanHex(char *str, int n)
{
    int bytes = n >> 1;
    unsigned char *ret = malloc(bytes);
    memset(ret, 0, bytes);

    for (int i = 0, i2 = 0; i < bytes; i++, i2 += 2)
    {
        // get value
        for (int j = 0; j < 2; j++)
        {
            ret[i] <<= 4;
            unsigned char c = str[i2 + j];
            if (c >= '0' && c <= '9')
            {
                ret[i] += c - '0';
            }
            else if (c >= 'a' && c <= 'f')
            {
                ret[i] += c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F')
            {
                ret[i] += c - 'A' + 10;
            }
            else
            {
                free(ret);
                return NULL;
            }
        }
    }

    return ret;
}

int main()
{
    printf("Hello, world!\n");

    unsigned char *msg = NULL;
    unsigned char *hash = NULL;

    const int N = 352;
    msg = scanHex("1F42ADD25C0A80A4C82AAE3A0E302ABF9261DCA7E7884FD869D96ED4CE88AAAA25304D2D79E1FA5CC1FA2C95899229BC87431AD06DA524F2140E70BD0536E9685EE78089590A012384709182357091283479AB93489182CD7397F12B261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A261DCA7E7884FD869D96ED4CE88A", N);
    if (msg)
    {
        int len = sha3_hash(msg, N >> 1, SHA3_256, &hash);
        printf("%d\n", len);
        printCharArr(hash, len, false);
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

    printCharArr(hash, SHA1_OUT, false);

    free(msg);
    free(hash);

    return 0;
}