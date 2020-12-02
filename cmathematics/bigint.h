#include <stdarg.h>
#include <stdio.h>

#include "cmathematics.h"

#ifndef BIGINT_H
#define BIGINT_H

/**
 * structure representing an integer
 * each character in the array corresponds to one digit in base 10
 */
typedef struct
{
    bool sign;             // the sign of the number (true if 0 or positive, false if negative)
    unsigned int capacity; // number of slots in the array
    unsigned int noDigits; // number of used slots in the array
    char *digits;          // array of the digits in reverse order; digits[i] = 10^i component
} bigint;

extern const bigint BIGINT_ZERO; // zero integer

/**
 * allocate memory for the integer
 * @param capacity the initial size of the array
 * @return the integer
 */
bigint allocateBigint(unsigned int capacity);

/**
 * convert string to a big integer by parsing each digit
 * @param str the string
 * @return the integer
 */
bigint strToBigint(char *str);

/**
 * creates new big integer from an integer
 * @param i the integer
 * @return the big integer
 */
bigint newBigint(int i);

/**
 * creates new positive big integer from an unsigned integer
 * @param i the integer
 * @return the integer
 */
bigint newPositiveBigint(unsigned int i);

/**
 * get string representation of number
 * @param i the pointer to the big integer
 * @return the string
 */
char *bigintPtrToString(bigint *i);

/**
 * get string representation of number
 * @param i the big integer
 * @return the string
 */
char *bigintToString(bigint i);

/**
 * comparison of two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return -1 if i1 < i2, 0 if i1 = i2, 1 if i1 > i2
 */
char compareBigint(bigint i1, bigint i2);

/**
 * adds two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the sum
 */
bigint addBigint(bigint i1, bigint i2);

/**
 * subtracts two integers
 * @param i1 the integer to subtract from
 * @param i2 the integer to subtract
 * @return the difference
 */
bigint subtractBigint(bigint i1, bigint i2);

#endif