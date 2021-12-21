#include "cmathematics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * method to divide and get the remainder
 * @param num the number to divide by
 * @param divisor the divisor
 * @param quotient the pointer to where to store the quotient
 * @param remainder the pointer to where to store the remainder
 */
void divMod(unsigned int num, unsigned int divisor, unsigned int *quotient, unsigned int *remainder)
{
    div_t res = div(num, divisor);
    *quotient = res.quot;
    *remainder = res.rem;
}

/**
 * method to divide and get the remainder with long long
 * @param num the number to divide by
 * @param divisor the divisor
 * @param quotient the pointer to where to store the quotient
 * @param remainder the pointer to where to store the remainder
 */
void divModLL(unsigned long long num, unsigned long long divisor, unsigned long long *quotient, unsigned long long *remainder)
{
    lldiv_t res = lldiv(num, divisor);
    *quotient = res.quot;
    *remainder = res.rem;
}

/**
 * method to calculate the number of digits required to represent an integer in a specific base
 * @param val the integer
 * @param base the base
 * @return the number of digits
 */
unsigned int numDigits(unsigned int val, unsigned int base)
{
    // base cannot be 1 or 0
    if (base <= 1)
    {
        return 0;
    }

    // result = ceil(log_base(val))

    unsigned int ret = 0;

    while (val)
    {
        ret++;
        val /= base;
    }

    return ret;
}

/**
 * method to calculate the number of digits required to represent an integer in a specific base with long long
 * @param val the integer
 * @param base the base
 * @return the number of digits
 */
unsigned long long numDigitsLL(unsigned long long val, unsigned long long base)
{
    // base cannot be 1 or 0
    if (base <= 1L)
    {
        return 0;
    }

    // result = ceil(log_base(val))

    unsigned long long ret = 0;

    while (val)
    {
        ret++;
        val /= base;
    }

    return ret;
}