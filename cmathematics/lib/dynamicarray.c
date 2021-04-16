#include "dynamicarray.h"

#include <stdlib.h>
#include <string.h>

#define DYNARR_DEFAULT_SIZE 4

dynamicarray dynarr_defaultAllocate()
{
    return dynarr_allocate(DYNARR_DEFAULT_SIZE);
}

dynamicarray dynarr_allocate(unsigned int capacity)
{
    if (!capacity)
    {
        capacity = DYNARR_DEFAULT_SIZE;
    }

    dynamicarray ret;

    // allocate memory for the array
    ret.list = malloc(capacity * sizeof(void *));
    ret.size = 0;
    ret.capacity = ret.list ? capacity : 0;

    return ret;
}

void dynarr_addAtIdx(dynamicarray *list, void *element, unsigned int idx)
{
    if (idx > list->size)
    {
        // boundary error
        return;
    }

    // resize if necessary
    dynarr_reallocate(list, 1);

    // right shift elements beyond index to make space
    for (unsigned int i = list->size; i > idx; i--)
    {
        list->list[i] = list->list[i - 1];
    }

    // set element at index
    list->list[idx] = element;
    list->size++;
}

void dynarr_addFirst(dynamicarray *list, void *element)
{
    dynarr_addAtIdx(list, element, 0);
}

void dynarr_addLast(dynamicarray *list, void *element)
{
    dynarr_addAtIdx(list, element, list->size);
}

void dynarr_set(dynamicarray *list, void *element, unsigned int idx)
{
    if (idx > list->size)
    {
        // boundary error
        return;
    }

    list->list[idx] = element;
}

void dynarr_reallocate(dynamicarray *list, unsigned int additionalLength)
{
    unsigned int newSize = list->size + additionalLength;

    if (newSize > list->capacity)
    {
        // get required capacity
        unsigned int capacity = list->capacity;
        if (!capacity)
        {
            capacity = 1;
        }
        while (capacity < newSize)
        {
            capacity <<= 1;
        }

        // must reallocate
        void **newMem = realloc(list->list, capacity * sizeof(void *));
        if (!newMem)
        {
            // allocate in new location
            newMem = malloc(capacity * sizeof(void *));
            memcpy(newMem, list->list, capacity * sizeof(void *));

            // update pointers
            free(list->list);
            list->list = newMem;
        }
        else if (newMem != list->list)
        {
            // reallocated in a new location
            // update pointers

            free(list->list);
            list->list = newMem;
        }

        list->capacity = capacity;
    }
}

void *dynarr_removeAtIdx(dynamicarray *list, unsigned int idx)
{
    // boundary check
    if (idx >= list->size)
    {
        return NULL;
    }

    // get the element to be removed
    void *ret = list->list[idx];

    // left shift elements
    for (unsigned int i = idx + 1; i < list->size; i++)
    {
        list->list[i - 1] = list->list[i];
    }
    // set last element to null
    list->list[list->size - 1] = NULL;

    // decrease size
    list->size--;

    return ret;
}

void *dynarr_removeFirst(dynamicarray *list)
{
    return dynarr_removeAtIdx(list, 0);
}

void *dynarr_removeLast(dynamicarray *list)
{
    return dynarr_removeAtIdx(list, list->size - 1);
}

void *dynarr_get(dynamicarray *list, unsigned int idx)
{
    // if in bounds, return element
    return (idx < list->size) ? list->list[idx] : NULL;
}

void dynarr_iterate(dynamicarray *list, void (*visit)(void *element))
{
    for (unsigned int i = 0; i < list->size; i++)
    {
        visit(list->list[i]);
    }
}

void dynarr_clear(dynamicarray *list)
{
    free(list->list);
    list->size = 0;
    list->capacity = DYNARR_DEFAULT_SIZE;
    list->list = malloc(DYNARR_DEFAULT_SIZE * sizeof(void *));
}

void dynarr_free(dynamicarray *list)
{
    free(list->list);
}

void dynarr_freeDeep(dynamicarray *list)
{
    free(list->list);
    for (unsigned int i = 0; i < list->size; i++)
    {
        free(list->list[i]);
    }
}

dynarr_iterator dynarr_iterator_new(dynamicarray *list)
{
    dynarr_iterator ret;

    ret.list = list;
    ret.cur_idx = 0;

    return ret;
}

void *dynarr_iterator_next(dynarr_iterator *it)
{
    return it->cur_idx < it->list->size ? it->list->list[it->cur_idx++] : NULL;
}