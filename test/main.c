#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"
#include "../cmathematics/shuntingyard.h"
#include "../cmathematics/graph.h"

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

void printIntArr(int *arr, int n)
{
    printf("[ ");

    for (int i = 0; i < n; i++)
    {
        printf("%d, ", arr[i]);
    }

    printf("]\n");
}

int main()
{
    printf("Hello, world!\n");

    bigint b1 = strToBigint("29318714789650187194709093218470912834709867109874321423947883712964819237");
    bigint b2 = strToBigint("12398704012381274312094710985714156298746129568167163279458172374619275132908741");

    bigint b3 = multiplyBigint(b1, b2);

    printf("%s * %s = %s\n", bigintToString(b1), bigintToString(b2), bigintToString(b3));

    freeBigint(&b1);
    freeBigint(&b2);
    freeBigint(&b3);

    return 0;
}