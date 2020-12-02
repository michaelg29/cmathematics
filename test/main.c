#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    bigint b1 = newBigint(-15932);
    bigint b2 = strToBigint("12334576129854641928374691283742109834709812374");

    printf("%s\n", bigintToString(subtractBigint(b1, b2)));

    return 0;
}