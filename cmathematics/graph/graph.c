#include "graph.h"
#include "../lib/strstream.h"
#include "../lib/minheap.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

edge *createEdge(int v1, int v2)
{
    edge *ret = malloc(sizeof(edge));

    ret->v1 = v1;
    ret->v2 = v2;
    ret->weight = 1;

    return ret;
}

edge *createWeightedEdge(int v1, int v2, int weight)
{
    edge *ret = malloc(sizeof(edge));

    ret->v1 = v1;
    ret->v2 = v2;
    ret->weight = weight;

    return ret;
}

// graph constructor/destructor
graph graph_new(char mode, int n)
{
    graph ret = {n, NULL, mode};

    // setup edge storage
    if (mode)
    {
        // allocate array of lists
        ret.adjacencyLists = malloc(n * sizeof(dynamicarray));
        for (int i = 0; i < n; i++)
        {
            // allocate each adjacency list
            ret.adjacencyLists[i] = dynarr_defaultAllocate();
        }
    }
    else
    {
        // allocate adjacency matrix
        ret.adjacencyMatrix = malloc(n * sizeof(int *));
        for (int i = 0; i < n; i++)
        {
            // allocate row
            ret.adjacencyMatrix[i] = malloc(n * sizeof(int));
            for (int j = 0; j < n; j++)
            {
                ret.adjacencyMatrix[i][j] = 0;
            }
        }
    }

    return ret;
}

void graph_free(graph *g)
{
    if (g)
    {
        if (g->adjacencyMode)
        {
            // free each list
            for (int i = 0; i < g->n; i++)
            {
                dynarr_freeDeep(g->adjacencyLists + i); // &g->adjacencyLists[i]
            }
            free(g->adjacencyLists);
        }
        else
        {
            for (int i = 0; i < g->n; i++)
            {
                free(g->adjacencyMatrix + i);
            }
            free(g->adjacencyMatrix);
        }
    }
}

graph graph_copy(graph *g)
{
    graph ret = {g->n, NULL, g->adjacencyMode};

    // copy edges
    if (g->adjacencyMode)
    {
        ret.adjacencyLists = malloc(g->n * sizeof(dynamicarray));
        for (int i = 0; i < ret.n; i++)
        {
            ret.adjacencyLists[i] = dynarr_allocate(g->adjacencyLists[i].size);
            for (int j = 0; j < g->adjacencyLists[i].size; j++)
            {
                edge *e = g->adjacencyLists[i].list[j];
                dynarr_addLast(ret.adjacencyLists + i, createWeightedEdge(e->v1, e->v2, e->weight));
            }
        }
    }
    else
    {
        ret.adjacencyMatrix = malloc(g->n * sizeof(int *));
        for (int i = 0; i < g->n; i++)
        {
            ret.adjacencyMatrix[i] = malloc(g->n * sizeof(int));
            memcpy(ret.adjacencyMatrix[i], g->adjacencyMatrix[i], g->n * sizeof(int));
        }
    }

    return ret;
}

// graph modifiers
void graph_addVertices(graph *g, int n)
{
    int nextN = g->n + n;

    if (g->adjacencyMode)
    {
        dynamicarray *oldMem = g->adjacencyLists; // get original pointer
        g->adjacencyLists = realloc(g->adjacencyLists, nextN * sizeof(dynamicarray));
        if (!g->adjacencyLists)
        {
            // reallocate in new location
            g->adjacencyLists = malloc(nextN * sizeof(dynamicarray));
            memcpy(g->adjacencyLists, oldMem, g->n * sizeof(dynamicarray));

            free(oldMem);

            // allocate new lists
            for (int i = 0; i < n; i++)
            {
                g->adjacencyLists[i + g->n] = dynarr_defaultAllocate();
            }
        }
    }
    else
    {
        int **newAdjacencyMatrix = malloc(nextN * sizeof(int *));
        for (int i = 0; i < nextN; i++)
        {
            newAdjacencyMatrix[i] = malloc(nextN * sizeof(int));
            if (i < g->n)
            {
                // copy existing values
                memcpy(newAdjacencyMatrix[i], g->adjacencyMatrix[i], g->n * sizeof(int));
                // set new values
                for (int j = 0; j < n; j++)
                {
                    newAdjacencyMatrix[i][j + g->n] = 0;
                }

                // free old memory
                free(g->adjacencyMatrix[i]);
            }
            else
            {
                // set new rows to hold all zeros
                for (int j = 0; j < nextN; j++)
                {
                    newAdjacencyMatrix[i][j] = 0;
                }
            }
        }
        // free old memory
        free(g->adjacencyMatrix);

        // update pointers
        g->adjacencyMatrix = newAdjacencyMatrix;
    }

    g->n = nextN;
}

void graph_addDirectedEdge(graph *g, int v1, int v2)
{
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createEdge(v1, v2));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = !0; // true
    }
}

void graph_addDirectedWeightedEdge(graph *g, int v1, int v2, int weight)
{
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createWeightedEdge(v1, v2, weight));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = weight; // true
    }
}

void graph_addUndirectedEdge(graph *g, int v1, int v2)
{
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createEdge(v1, v2));
        dynarr_addLast(g->adjacencyLists + v2, createEdge(v2, v1));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = !0;
        g->adjacencyMatrix[v2][v1] = !0;
    }
}

void graph_addUndirectedWeightedEdge(graph *g, int v1, int v2, int weight)
{
    if (g->adjacencyMode)
    {
        dynarr_addLast(g->adjacencyLists + v1, createWeightedEdge(v1, v2, weight));
        dynarr_addLast(g->adjacencyLists + v2, createWeightedEdge(v2, v1, weight));
    }
    else
    {
        g->adjacencyMatrix[v1][v2] = weight;
        g->adjacencyMatrix[v2][v1] = weight;
    }
}

// graph accessors
char *graph_toString(graph *g)
{
    strstream ret = strstream_alloc(g->n);

    // iterate through each vertex
    for (int i = 0; i < g->n; i++)
    {
        if (g->adjacencyMode)
        {
            edge *e = NULL;
            dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + i);

            // iterate through adjacency list
            while ((e = dynarr_iterator_next(&it)))
            {
                if (e->weight)
                {
                    strstream_concat(&ret, "%d -> %d; %d\n", e->v1, e->v2, e->weight);
                }
            }
        }
        else
        {
            // scan through row, output edges
            for (int j = 0; j < g->n; j++)
            {
                if (g->adjacencyMatrix[i][j])
                {
                    strstream_concat(&ret, "%d -> %d; %d\n", i, j, g->adjacencyMatrix[i][j]);
                }
            }
        }
    }

    return ret.str;
}

// DFS
void graph_dfsStart(graph *g, int src, int *d, int *f, int *p)
{
    int time = 0;
    for (int i = 0; i < g->n; i++)
    {
        d[i] = 0;
        f[i] = 0;
        p[i] = -1;
    }

    graph_dfs(g, src, d, f, p, &time);
}

void graph_dfs(graph *g, int src, int *d, int *f, int *p, int *time)
{
    (*time)++;
    d[src] = *time;

    // discover neighbors
    if (g->adjacencyMode)
    {
        edge *e = NULL;
        dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + src);

        // iterate through edges
        while ((e = dynarr_iterator_next(&it)))
        {
            if (e->weight && !d[e->v2])
            {
                // edge has weight (valid), second vertex not discovered yet
                graph_dfs(g, e->v2, d, f, p, time);
                p[e->v2] = src;
            }
        }
    }
    else
    {
        // scan through rows
        for (int i = 0; i < g->n; i++)
        {
            if (g->adjacencyMatrix[src][i] && !d[i])
            {
                // edge has weight (valid), second vertex not discovered yet
                graph_dfs(g, i, d, f, p, time);
                p[i] = src;
            }
        }
    }

    (*time)++;
    f[src] = *time;
}

int graph_pathDfsStart(graph *g, int src, int dst, int *p)
{
    bool *visited = malloc(g->n * sizeof(bool));
    for (int i = 0; i < g->n; i++)
    {
        visited[i] = 0;
        p[i] = -1;
    }

    int ret = graph_pathDfs(g, src, dst, visited, p);

    free(visited);

    return ret;
}

int graph_pathDfs(graph *g, int src, int dst, bool *visited, int *p)
{
    if (!visited[src])
    {
        // source not visited yet

        // mark the source as visited
        visited[src] = true;

        if (g->adjacencyMode)
        {
            edge *e = NULL;
            dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + src);

            while ((e = dynarr_iterator_next(&it)))
            {
                if (e->weight && !visited[e->v2])
                {
                    // edge from src to e->v2 has weight, e->v2 not discovered
                    if (e->v2 == dst)
                    {
                        // found goal
                        p[dst] = src;
                        return e->weight;
                    }
                    else
                    {
                        // recurse along edge
                        int weight = graph_pathDfs(g, e->v2, dst, visited, p);
                        if (weight)
                        {
                            // found a path to the destination
                            p[e->v2] = src;
                            // return smaller weight
                            return MIN(e->weight, weight);
                        }
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < g->n; i++)
            {
                if (g->adjacencyMatrix[src][i] && !visited[i])
                {
                    if (i == dst)
                    {
                        // found goal
                        p[dst] = src;
                        return g->adjacencyMatrix[src][i];
                    }
                    else
                    {
                        // recurse along edge
                        int weight = graph_pathDfs(g, i, dst, visited, p);
                        if (weight)
                        {
                            // found a path to the destination
                            p[i] = src;
                            // return smaller weight
                            return MIN(g->adjacencyMatrix[src][i], weight);
                        }
                    }
                }
            }
        }
    }

    // found no path
    return 0;
}

typedef struct
{
    int v;       // vertex number
    int *weight; // weight to the vertex (serves as the key)
} dijkstra_node;

dijkstra_node *create_dijkstra_node(int node, int *weight)
{
    dijkstra_node *ret = malloc(sizeof(dijkstra_node));

    ret->v = node;
    ret->weight = weight;

    return ret;
}

int dijkstra_node_compare(void *v1, void *v2)
{
    dijkstra_node *n1 = v1;
    dijkstra_node *n2 = v2;

    int w1 = *(n1->weight);
    int w2 = *(n2->weight);

    if (w1 > w2)
    {
        return 1;
    }
    else if (w1 == w2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int dijkstra_node_index(void *v)
{
    return ((dijkstra_node *)v)->v;
}

// algorithms
int *graph_dijkstra(graph *g, int src, bool isTargeted, int target)
{
    /*
        INITIALIZATION
    */
    int *d = malloc(g->n * sizeof(int));
    int *p = malloc(g->n * sizeof(int));

    minheap q = mheap_allocate(dijkstra_node_compare);

    // initialize the source values
    d[src] = 0;
    p[src] = src;
    mheap_add(&q, create_dijkstra_node(src, d + src));

    // add other nodes
    for (int i = 0; i < g->n; i++)
    {
        if (i == src)
        {
            continue;
        }

        d[i] = INT_MAX; // infinity
        p[i] = -1;
        mheap_add(&q, create_dijkstra_node(i, d + i));
    }

    mheap_attachIndexMap(&q, 0, dijkstra_node_index);

    /*
        MAIN LOOP
    */
    dijkstra_node *cur = NULL;
    while ((cur = mheap_pop(&q)))
    {
        if (isTargeted && cur->v == target) {
            // found shortest path to target from src
            break;
        }

        // relax edges from the popped node
        if (g->adjacencyMode)
        {
            edge *e = NULL;
            dynarr_iterator it = dynarr_iterator_new(g->adjacencyLists + cur->v);

            while ((e = dynarr_iterator_next(&it)))
            {
                int v = e->v2;
                if (cur->v != v && q.indexMap[v] != -1)
                {
                    // cur is not v, v is not finished
                    // determine if lighter path to v through cur
                    if (d[v] > d[cur->v] + e->weight)
                    {
                        // update weight
                        d[v] = d[cur->v] + e->weight;
                        p[v] = cur->v;

                        // call upheap on the new weight
                        mheap_upheap(&q, q.indexMap[v]);
                    }
                }
            }
        }
        else
        {
            for (int v = 0; v < g->n; v++)
            {
                if (cur->v != v && g->adjacencyMatrix[cur->v][v] && q.indexMap[v] != -1)
                {
                    // cur is not v, edge exists from cur to v, v is not finished
                    // determine if lighter path to v through cur
                    if (d[v] > d[cur->v] + g->adjacencyMatrix[cur->v][v])
                    {
                        // update weight
                        d[v] = d[cur->v] + g->adjacencyMatrix[cur->v][v];
                        p[v] = cur->v;

                        // call upheap on the new weight
                        mheap_upheap(&q, q.indexMap[v]);
                    }
                }
            }
        }

        free(cur);
    }

    /*
        CLEANUP
    */
    mheap_freeDeep(&q);
    free(d);

    // return predecessor array
    return p;
}

int *graph_targetedDijkstra(graph *g, int src, int target) {
    return graph_dijkstra(g, src, true, target);
}

int *graph_generalDijkstra(graph *g, int src) {
    return graph_dijkstra(g, src, false, 0);
}

graph graph_fordFulkerson(graph *g, int src, int dst, int *maxFlowRet)
{
    graph ret = graph_copy(g);
    graph resid = graph_copy(g);
    int maxFlow = 0;

    int *p = malloc(g->n * sizeof(int));
    int pathFlow; // bottleneck of each path
    while ((pathFlow = graph_pathDfsStart(&resid, src, dst, p)))
    {
        // while a path exists from the source to the destination (augmenting path)
        for (int i = dst; i != src; i = p[i])
        {
            // edge from p[i] to i is in the augmenting path
            if (g->adjacencyMode)
            {
                // find the forward edge
                edge *e = NULL;
                dynarr_iterator it = dynarr_iterator_new(resid.adjacencyLists + p[i]);

                while ((e = dynarr_iterator_next(&it)))
                {
                    if (e->v2 == i)
                    {
                        // decrease remaining capacity on the forward edge
                        e->weight -= pathFlow;
                        break;
                    }
                }

                // find residual edge
                bool residualFound = false;
                e = NULL;
                it = dynarr_iterator_new(resid.adjacencyLists + i);

                while ((e = dynarr_iterator_next(&it)))
                {
                    if (e->v2 == p[i])
                    {
                        residualFound = true;
                        e->weight += pathFlow;
                        break;
                    }
                }

                if (!residualFound)
                {
                    dynarr_addLast(resid.adjacencyLists + i,
                                   createWeightedEdge(i, p[i], pathFlow));
                }
            }
            else
            {
                // decrease weight by path flow (remaining capacity)
                resid.adjacencyMatrix[p[i]][i] -= pathFlow;
                // add weight to the residual edge
                resid.adjacencyMatrix[i][p[i]] += pathFlow;
            }
        }
        maxFlow += pathFlow;
    }
    free(p);

    // subtract remaining capacities from original capacities to get attained flow
    for (int i = 0; i < ret.n; i++)
    {
        if (ret.adjacencyMode)
        {
            for (int j = 0; j < ret.adjacencyLists[i].size; j++)
            {
                edge *retEdge = ret.adjacencyLists[i].list[j];
                edge *residEdge = resid.adjacencyLists[i].list[j];

                retEdge->weight -= residEdge->weight;
            }
        }
        else
        {
            for (int j = 0; j < ret.n; j++)
            {
                if (ret.adjacencyMatrix[i][j])
                {
                    ret.adjacencyMatrix[i][j] -= resid.adjacencyMatrix[i][j];
                }
            }
        }
    }

    if (maxFlowRet)
    {
        *maxFlowRet = maxFlow;
    }

    graph_free(&resid);

    return ret;
}