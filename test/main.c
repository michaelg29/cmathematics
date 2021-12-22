#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/lib/arrays.h"
#include "../cmathematics/util/numio.h"
#include "../cmathematics/data/encryption/aes.h"

int main() {
    char in[4] = "abcd";
    int val = smallEndianValue(in, 4);
    printf("%d, %d, %d, %d => %d\n", in[0], in[1], in[2], in[3], val);

    char *out = smallEndianStr(val);
    printf("%s, %d\n", out, strlen(out));
    free(out);

    srand(time(0));
    //unsigned char *iv = newRandomBytes(16);
    unsigned char iv[16];
    memset(iv, 0xFF, 16);
    //unsigned int inc = ~0;
    unsigned int inc = 1;

    out = printByteArr(iv, 16, " ", 1, 1);
    printf("{%s}\n", out);
    free(out);

    out = largeEndianStr(inc);
    out = printByteArr(out, 4, " ", 1, 1);
    printf("{                                    %s}\n", out);
    free(out);

    aes_incrementCounter(iv, inc);
    out = printByteArr(iv, 16, " ", 1, 1);
    printf("{%s}\n", out);
    free(out);

    return 0;
}