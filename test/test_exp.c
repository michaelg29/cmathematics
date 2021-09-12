#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/util/expressions.h"

void printToken(void *el)
{
    exp_token *t = el;
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

    exp_init();

    dynamicarray rpn = exp_RPN("sin(cos(3pi/2 + 15.0 / 20.1*exp(15)))");
    exp_tokenNode *tree = exp_getEquationTree(rpn);
    double res = exp_evalTree(tree);
    printf("%f\n", res);
    exp_freeNodeDeep(tree);
    exp_freeTokenList(&rpn);

    exp_cleanup();
}