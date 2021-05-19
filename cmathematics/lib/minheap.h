#ifndef MINHEAP_H
#define MINHEAP_H

#define DEFAULT_ARRAY_SIZE 4

typedef struct minheap
{
    unsigned int capacity;
    unsigned int size;

    void **heap;

    int (*entrycmp)(void *val1, void *val2);
} minheap;

minheap mheap_allocate(int (*entrycmp)(void *val1, void *val2));
void mheap_reallocate(minheap *heap);

void mheap_swapElements(minheap *heap, unsigned int i1, unsigned int i2);
int mheap_entrycmp(minheap *heap, unsigned int i1, unsigned int i2);

void mheap_add(minheap *heap, void *val);
void mheap_upheap(minheap *heap, unsigned int idx);

void *mheap_pop(minheap *heap);
void mheap_downheap(minheap *heap, unsigned int idx);

void mheap_free(minheap *heap);
void mheap_freeDeep(minheap *heap);

#endif