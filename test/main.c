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

    if (SY_registerVariable("pi", 2.0))
    {
        printf("Registered into variable\n");
    }
    else
    {
        printf("Failed to register\n");
    }

    if (SY_registerVariable("xc", 2.0))
    {
        printf("Registered into variable\n");
    }
    else
    {
        printf("Failed to register\n");
    }

    // -1 * sin(2 * pi * cos(2 * e) - sqrt(22)) + -1
    dynamicarray tokenList = RPN("-sin(2picos(2e) - sqrt(22))+-1-xc");
    dynarr_iterate(&tokenList, printToken);

    SY_tokenNode *tree = getEquationTree(tokenList);
    printf("\n%.8f\n", evalTree(tree));

    SY_freeNode(tree);
    SY_freeTokenList(&tokenList);

    sy_cleanup();

    return 0;
}