#include "cmathematics.h"

void swap(int *a, int *b)
{
    *a ^= *b;
    *b = *a ^ *b;
    *a ^= *b;
}

void swapFloat(float *a, float *b)
{
    swap((int *)a, (int *)b);
}