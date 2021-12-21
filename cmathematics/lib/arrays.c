#include "arrays.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * method to determine if an integer exists in an array
 * @param arr the array
 * @param noElements the number of elements in the array
 * @param target the target element
 * @return if the element exists in the array
 */
char arrContains(void **arr, int n, void *target)
{
    for (unsigned int i = 0; i < n; i++)
    {
        if (arr[i] == target)
        {
            return 1;
        }
    }

    return 0;
}

// random array generation
char *newRandomBytes(int n)
{
    char *ret = malloc(n);
    if (!ret)
    {
        return NULL;
    }

    randomBytes(ret, n);
    return ret;
}

void randomBytes(char *out, int n)
{
    if (!out || !n || n < 0)
    {
        return;
    }

    while (n--)
    {
        out[n - 1] = rand() & 0xff; // cap at 255
    }
}