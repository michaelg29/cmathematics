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

    // bigint b1 = strToBigint("293187123947883712964819237");
    // bigint b2 = strToBigint("123987040123875132908741");

    // bigint b3 = multiplyBigint(b1, b2);

    // printf("%s * %s = %s\n", bigintToString(b1), bigintToString(b2), bigintToString(b3));

    // freeBigint(&b1);
    // freeBigint(&b2);
    // freeBigint(&b3);

    int N = 5;
    graph g = graph_new(ADJ_MATRIX, N);

    graph_addDirectedWeightedEdge(&g, 0, 1, 5);
    graph_addUndirectedWeightedEdge(&g, 0, 2, 13);
    graph_addDirectedEdge(&g, 2, 3);
    graph_addDirectedWeightedEdge(&g, 3, 1, 6);
    graph_addDirectedWeightedEdge(&g, 3, 4, 78);

    char *str = graph_toString(&g);
    printf("%s\n", str);
    free(str);

    int *d = malloc(N * sizeof(int));
    int *f = malloc(N * sizeof(int));
    int *p = malloc(N * sizeof(int));

    graph_dfsStart(&g, 0, d, f, p);

    graph_free(&g);

    printIntArr(d, N);
    printIntArr(f, N);
    printIntArr(p, N);

    free(d);
    free(f);
    free(p);

    return 0;
}