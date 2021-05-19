#include "minheap.h"

#include <stdlib.h>
#include <stdio.h>

minheap mheap_allocate(int (*entrycmp)(void *val1, void *val2))
{
    minheap ret;

    ret.capacity = DEFAULT_ARRAY_SIZE;
    ret.size = 0;

    ret.heap = malloc(ret.capacity * sizeof(void *));
    ret.heap[0] = NULL;

    ret.entrycmp = entrycmp;

    return ret;
}

void mheap_reallocate(minheap *heap)
{
    if (heap->size >= heap->capacity)
    {
        unsigned int newLength = heap->capacity;
        while (newLength <= heap->size)
        {
            newLength <<= 1;
        }

        // create new array
        void **newArr = malloc(newLength * sizeof(void *));
        // copy elements
        for (unsigned int i = 0; i < heap->size; i++)
        {
            newArr[i] = heap->heap[i];
        }

        // update pointers and vals
        void **tmpArr = heap->heap;
        heap->heap = newArr;
        newArr = tmpArr;
        free(newArr);

        heap->capacity = newLength;
    }
}

void mheap_attachIndexMap(minheap *heap, int n, int (*indexFunc)(void *v))
{
    if (!n)
    {
        n = heap->size;
    }

    if (heap->indexMap)
    {
        free(heap->indexMap);
    }

    heap->indexMap = malloc(n * sizeof(int));
    heap->indexFunc = indexFunc;

    for (int i = 1; i < heap->size; i++)
    {
        heap->indexMap[indexFunc(heap->heap[i])] = i;
    }
}

void mheap_updateIndex(minheap *heap, void *e, int idx)
{
    if (heap->indexMap && heap->indexFunc && e)
    {
        heap->indexMap[heap->indexFunc(e)] = idx;
    }
}

void mheap_swapElements(minheap *heap, unsigned int i1, unsigned int i2)
{
    void *tmp = heap->heap[i1];
    heap->heap[i1] = heap->heap[i2];
    heap->heap[i2] = tmp;

    mheap_updateIndex(heap, heap->heap[i1], i1);
    mheap_updateIndex(heap, heap->heap[i2], i2);
}

int mheap_entrycmp(minheap *heap, unsigned int i1, unsigned int i2)
{
    if (i1 > heap->size || i2 > heap->size)
    {
        return 0;
    }

    return heap->entrycmp(heap->heap[i1], heap->heap[i2]);
}

void mheap_add(minheap *heap, void *val)
{
    // increase size, resize
    heap->size++;
    mheap_reallocate(heap);

    // set next open slot to val
    heap->heap[heap->size] = val;
    mheap_updateIndex(heap, val, heap->size);

    mheap_upheap(heap, heap->size);
}

void mheap_upheap(minheap *heap, unsigned int idx)
{
    // lower bound
    // compare child (at idx) with parent (idx / 2)
    while (idx > 1 && mheap_entrycmp(heap, idx, idx / 2) < 0)
    {
        // swap with and track parent (move up)
        mheap_swapElements(heap, idx, idx / 2);
        idx >>= 1;
    }
}

void *mheap_pop(minheap *heap)
{
    void *ret = heap->heap[1]; // get root node
    mheap_updateIndex(heap, ret, -1);

    if (ret && heap->size > 1)
    {
        // substitute with last node
        heap->heap[1] = heap->heap[heap->size--];
        mheap_updateIndex(heap, heap->heap[1], 1);
        mheap_downheap(heap, 1);
    }
    else if (heap->size == 1)
    {
        heap->heap[1] = NULL;
    }
    else if (!heap->size)
    {
        return NULL;
    }

    return ret;
}

void mheap_downheap(minheap *heap, unsigned int idx)
{
    unsigned int child;
    while ((child = 2 * idx) <= heap->size) // left child exists
    {
        if (child < heap->size) // has right child
        {
            // want to swap with smaller child
            if (mheap_entrycmp(heap, child + 1, child) < 0) // right child is smaller
            {
                child++;
            }
        }

        // if child swap is necessary
        if (mheap_entrycmp(heap, child, idx) < 0) // smaller child is smaller than the parent
        {
            // swap with and track child (move down)
            mheap_swapElements(heap, idx, child);
            idx = child;
        }
        else
        {
            // properly sorted
            break;
        }
    }
}

void mheap_free(minheap *heap)
{
    free(heap->heap);
    heap->size = 0;
    heap->capacity = 0;

    if (heap->indexMap)
    {
        free(heap->indexMap);
    }
}

void mheap_freeDeep(minheap *heap)
{
    for (unsigned int i = 1; i <= heap->size; i++)
    {
        free(heap->heap[i]);
    }

    mheap_free(heap);
}