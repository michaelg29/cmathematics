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
    printf("%s => %s\n", str, base64);
    free(base64);

    return 0;
}