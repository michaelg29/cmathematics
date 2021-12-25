#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/graph/graph.h"

int main()
{
    printf("Hello, world!\n");

    graph g = graph_new(ADJ_LISTS, 6);

    graph_addDirectedWeightedEdge(&g, 0, 1, 10);
    graph_addDirectedWeightedEdge(&g, 0, 3, 8);
    graph_addDirectedWeightedEdge(&g, 1, 2, 5);
    graph_addDirectedWeightedEdge(&g, 1, 3, 2);
    graph_addDirectedWeightedEdge(&g, 2, 5, 7);
    graph_addDirectedWeightedEdge(&g, 3, 4, 10);
    graph_addDirectedWeightedEdge(&g, 4, 2, 8);
    graph_addDirectedWeightedEdge(&g, 4, 5, 10);

    char *str = graph_toString(&g);
    printf("%s\n", str);
    free(str);

    int maxFlow = 0;
    graph ffgraph = graph_fordFulkerson(&g, 0, 5, &maxFlow);
    str = graph_toString(&ffgraph);
    printf("%d\n%s\n", maxFlow, str);
    free(str);

    graph_free(&g);
    graph_free(&ffgraph);

    return 0;
}