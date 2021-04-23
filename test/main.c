#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"
#include "../cmathematics/shuntingyard.h"

void printToken(void *el)
{
    SY_token *t = el;
    switch (t->type)
    {
    case CONSTANT:
        printf("%f ", t->val.constVal);
        break;
    case FUNCTION:
    case OPERATOR:
        printf("%s ", t->val.funcVal.name);
        break;
    case CONSTANTSTR:
        printf("%s ", t->val.namedConstVal.name);
        break;
    case ELSE:
        printf("null ");
        break;
    default:
        printf("%s ", t->val.strVal);
        break;
    }
}

int main()
{
    printf("Hello, world!\n");

    // bigint b1 = strToBigint("293187123947883712964819237");
    // bigint b2 = strToBigint("123987040123875132908741");

    // bigint b3 = multiplyBigint(b1, b2);

    // printf("%s * %s = %s\n", bigintToString(b1), bigintToString(b2), bigintToString(b3));

    // freeBigint(&b1);
    // freeBigint(&b2);
    // freeBigint(&b3);

    sy_init();

    // -1 * sin(2 * pi * cos(2 * e) - sqrt(22)) + -1
    dynamicarray tokenList = RPN("sin(3^-2*27/4pi)+1");
    dynarr_iterate(&tokenList, printToken);
    dynarr_free(&tokenList);

    sy_cleanup();

    return 0;
}