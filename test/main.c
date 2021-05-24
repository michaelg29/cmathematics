#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"
#include "../cmathematics/expressions.h"
#include "../cmathematics/graph.h"

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

    exp_init();

    dynamicarray rpn = exp_RPN("sin(cos(3pi/2 + 15.0 / 20.1*exp(15)))");
    exp_tokenNode *tree = exp_getEquationTree(rpn);
    double res = exp_evalTree(tree);
    printf("%f\n", res);
    exp_freeNodeDeep(tree);
    exp_freeTokenList(&rpn);

    exp_cleanup();

    // graph g = graph_new(ADJ_MATRIX, 7);

    // graph_addDirectedWeightedEdge(&g, 0, 1, 6);
    // graph_addDirectedWeightedEdge(&g, 0, 3, 5);
    // graph_addDirectedWeightedEdge(&g, 1, 2, 7);
    // graph_addDirectedWeightedEdge(&g, 1, 3, 3);
    // graph_addDirectedWeightedEdge(&g, 2, 4, 15);
    // graph_addDirectedWeightedEdge(&g, 2, 5, 2);
    // graph_addDirectedWeightedEdge(&g, 3, 2, 2);
    // graph_addDirectedWeightedEdge(&g, 3, 4, 11);
    // graph_addDirectedWeightedEdge(&g, 4, 6, 3);
    // graph_addDirectedWeightedEdge(&g, 5, 4, 1);
    // graph_addDirectedWeightedEdge(&g, 5, 6, 6);

    // char *str = graph_toString(&g);
    // printf("%s\n", str);
    // free(str);

    // int *p = graph_dijkstra(&g, 0);
    // printIntArr(p, g.n);
    // free(p);

    // graph_free(&g);

    return 0;
}