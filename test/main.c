#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    bigint b1 = strToBigint("293187123947883712964819237");
    bigint b2 = strToBigint("123987040123875132908741");

    bigint b3 = multiplyBigint(b1, b2);

    printf("%s * %s = %s\n", bigintToString(b1), bigintToString(b2), bigintToString(b3));

    freeBigint(&b1);
    freeBigint(&b2);
    freeBigint(&b3);

    return 0;
}