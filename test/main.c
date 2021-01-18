#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    bigint b1 = strToBigint("123456789780");
    bigint b2 = strToBigint("123856789780");
    bigint b3 = addBigint(b1, b2);
    bigint b4 = subtractBigint(b3, b1);

    printf("%s + %s = %s\n", bigintToString(b1), bigintToString(b2), bigintToString(b3));
    printf("%s - %s = %s\n", bigintToString(b3), bigintToString(b1), bigintToString(b4));

    return 0;
}