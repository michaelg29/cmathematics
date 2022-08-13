#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/lib/arrays.h"
#include "../cmathematics/util/numio.h"

int main() {
    unsigned char *str = "helloa;fsldkfj afhau;sdf";
    unsigned char *base64 = base64_encode(str, strlen(str));
    unsigned char *decode = base64_decode(base64, strlen(base64));
    printf("%s => %s => %s\n", str, base64, decode);
    free(base64);
    free(decode);

    return 0;
}