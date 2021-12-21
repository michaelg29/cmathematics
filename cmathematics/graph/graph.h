#include "../lib/dynamicarray.h"
#include "../cmathematics.h"

#ifndef GRAPH_H
#define GRAPH_H

#define ADJ_MATRIX 0 // false
#define ADJ_LISTS !0 // true

typedef struct
{
    // number of vertices
    int n;

    // edge storage
    union
    {
        int **adjacencyMatrix;
        dynamicarray *adjacencyLists;
    };
    char adjacencyMode;
} graph;

typedef struct
{
    int v1;
    int v2;
    int weight;
} edge;

// edge constructors
edge *createEdge(int v1, int v2);
edge *createWeightedEdge(int v1, int v2, int weight);

// graph constructor/destructor
graph graph_new(char mode, int n);
void graph_free(graph *g);
graph graph_copy(graph *g);

// graph modifiers
void graph_addVertices(graph *g, int n);
void graph_addDirectedEdge(graph *g, int v1, int v2);
void graph_addDirectedWeightedEdge(graph *g, int v1, int v2, int weight);
void graph_addUndirectedEdge(graph *g, int v1, int v2);
void graph_addUndirectedWeightedEdge(graph *g, int v1, int v2, int weight);

// graph accessors
char *graph_toString(graph *g);

// DFS
void graph_dfsStart(graph *g, int src, int *d, int *f, int *p);
void graph_dfs(graph *g, int src, int *d, int *f, int *p, int *time);
int graph_pathDfsStart(graph *g, int src, int dst, int *p);
int graph_pathDfs(graph *g, int src, int dst, bool *visited, int *p);

// algorithms
int *graph_dijkstra(graph *g, int src, bool isTargeted, int target);
int *graph_targetedDijkstra(graph *g, int src, int target);
int *graph_generalDijkstra(graph *g, int src);
graph graph_fordFulkerson(graph *g, int src, int dst, int *maxFlowRet);

#endif // GRAPH_H