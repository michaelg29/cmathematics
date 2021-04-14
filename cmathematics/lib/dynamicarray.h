#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

typedef struct
{
    unsigned int size;
    unsigned int capacity;
    void **list;
} dynamicarray;

dynamicarray dynarr_defaultAllocate();
dynamicarray dynarr_allocate(unsigned int capacity);

void dynarr_addAtIdx(dynamicarray *list, void *element, unsigned int idx);
void dynarr_addFirst(dynamicarray *list, void *element);
void dynarr_addLast(dynamicarray *list, void *element);
void dynarr_reallocate(dynamicarray *list, unsigned int additionalLength);

void dynarr_set(dynamicarray *list, void *element, unsigned int idx);

void *dynarr_removeAtIdx(dynamicarray *list, unsigned int idx);
void *dynarr_removeFirst(dynamicarray *list);
void *dynarr_removeLast(dynamicarray *list);

void *dynarr_get(dynamicarray *list, unsigned int idx);

void dynarr_iterate(dynamicarray *list, void (*visit)(void *element));

#define dynarr_createSpecificIterator(type)                                         \
    void dynarr_##type##_iterate(dynamicarray *list, void (*visit)(type * element)) \
    {                                                                               \
        for (unsigned int i = 0; i < list->size; i++)                               \
        {                                                                           \
            visit((type *)list->list[i]);                                           \
        }                                                                           \
    }

void dynarr_clear(dynamicarray *list);

void dynarr_free(dynamicarray *list);
void dynarr_freeDeep(dynamicarray *list);

typedef struct dynarr_iterator
{
    dynamicarray *list;
    unsigned int cur_idx;
} dynarr_iterator;

dynarr_iterator dynarr_iterator_new(dynamicarray *list);
void *dynarr_iterator_next(dynarr_iterator *it);

#endif