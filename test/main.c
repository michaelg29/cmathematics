#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/util/numio.h"

int main() {
    char in[4] = "abcd";
    int val = smallEndianValue(in, 4);
    printf("%d, %d, %d, %d => %d\n", in[0], in[1], in[2], in[3], val);

    char *out = smallEndianStr(val);
    printf("%s, %d\n", out, strlen(out));
    free(out);

    return 0;
}