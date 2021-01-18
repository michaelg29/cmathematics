#include "cmathematics.h"
#include <stdio.h>

/**
 * method to determine if an integer exists in an array
 * @param arr the array
 * @param noElements the number of elements in the array
 * @param target the target element
 * @return if the element exists in the array
 */
bool containsUint(unsigned int *arr, unsigned int noElements, unsigned int target)
{
    for (unsigned int i = 0; i < noElements; i++)
    {
        if (arr[i] == target)
        {
            return true;
        }
    }

    return false;
}

/**
 * method to print out an integer array
 * @param arr the array
 * @param noElements the number of elements in the array
 */
void printUintArray(unsigned int *arr, unsigned int noElements)
{
    printf("[");
    for (unsigned int i = 0; i < noElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

/**
 * method to divide and get the remainder
 * @param num the number to divide by
 * @param divisor the divisor
 * @param quotient the pointer to where to store the quotient
 * @param remainder the pointer to where to store the remainder
 */
void divMod(unsigned int num, unsigned int divisor, unsigned int *quotient, unsigned int *remainder)
{
    *quotient = 0;

    while (num >= divisor)
    {
        // subtract one multiple
        num -= divisor;

        // add one to the quotient
        (*quotient)++;
    }

    // the rest of the value is the remainder
    *remainder = num;
}

/**
 * method to calculate the number of digits required to represent an integer in a specific base
 * @param val the integer
 * @param base the base
 * @return the number of digits
 */
unsigned int numDigits(unsigned int val, unsigned int base)
{
    // result = ceil(log_base(val))

    unsigned int ret = 0;

    while (val)
    {
        ret++;
        val /= base;
    }

    return ret;
}