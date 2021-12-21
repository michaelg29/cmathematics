#include <stdarg.h>

#include "../cmathematics.h"

#ifndef BIGINT_H
#define BIGINT_H

#define BASE 1000000000       // base used to represent each digit in the big integer
#define NO_BASE_DIGITS 9      // number of base 10 digits the base uses
#define KARATSUBA_THRESHOLD 5 // threshold between performing long and karatsuba multiplication

/**
 * structure representing an integer
 * each integer in the array corresponds to one digit in base 10^9
 */
typedef struct
{
    bool sign;             // the sign of the number (true if 0 or positive, false if negative)
    unsigned int capacity; // number of slots in the array
    unsigned int noDigits; // number of used slots in the array
    int *digits;           // array of the digits in reverse order; digits[i] = BASE^i component
} bigint;

extern const bigint BIGINT_ZERO;    // zero integer
extern const bigint BIGINT_ONE;     // one integer
extern const bigint BIGINT_NEG_ONE; // negative one integer

/**
 * allocate memory for the integer
 * @param capacity the initial size of the array
 * @return the integer
 */
bigint bigint_allocate(unsigned int capacity);

/**
 * copies an array of integers into a bigint structure
 * @param arr the array
 * @param n the number of digits
 * @param sign the sign of the bigint
 * @return the bigint structure
 */
bigint bigint_copyIntArr(int *arr, unsigned int n, bool sign);

/**
 * free the memory of a big integer
 * @param i the integer
 */
void bigint_free(bigint *i);

/**
 * convert string to a big integer by parsing each digit
 * 
 * PROCESS
 * for each block of 9 base 10 digits
 * val = digit[8] * 10^8 + digit[7] * 10^7 + ... + digit[0] + 10^0
 *     = ((digit[8] * 10 + digit[7]) * 10 + ... ) * 10 + digit[0]
 * 
 * @param str the string
 * @return the integer
 */
bigint bigint_fromString(char *str);

/**
 * creates new positive big integer from an unsigned integer
 * 
 * PROCESS - conversion of bases
 * i_10 / BASE = i' R digits[0] = LSD (least significant digit)
 * i' / BASE = i'' R digits[1]
 * ...
 * i''' / BASE = 0 R digits[N - 1] = MSD (most significant digit)
 * 
 * @param i the integer
 * @return the integer
 */
bigint bigint_fromUint(unsigned int i);

/**
 * creates new positive big integer from an unsigned long long
 * same process as bigint_fromUint
 * 
 * @param l the num
 * @return the big integer
 */
bigint bigint_fromULLint(unsigned long long l);

/**
 * creates new big integer from an integer
 * calls bigint_fromUint then applies sign after
 * @param i the integer
 * @return the big integer
 */
bigint bigint_fromInt(int i);

/**
 * creates new big integer from a long long
 * calls bigint_fromULLint then applies sign after
 * @param i the long long
 * @return the big integer
 */
bigint bigint_fromLLint(long long l);

/**
 * multiplies two integers together and handles possible overflow
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the big integer containing the product
 */
bigint bigint_fromUiMult(unsigned int i1, unsigned int i2);

/**
 * trims leading zeros in the array of digits
 * @param b the pointer to the integer to trim
 */
void bigint_trim(bigint *b);

/**
 * get string representation of number
 * @param i the big integer
 * @return the string
 */
char *bigint_toString(bigint i);

/**
 * comparison of two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return -1 if i1 < i2, 0 if i1 = i2, 1 if i1 > i2
 */
char bigint_compare(bigint i1, bigint i2);

/**
 * adds two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the sum
 */
bigint bigint_add(bigint i1, bigint i2);

/**
 * subtracts two integers
 * @param i1 the integer to subtract from
 * @param i2 the integer to subtract
 * @return the difference
 */
bigint bigint_subtract(bigint i1, bigint i2);

/**
 * multiplies two bigints
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the product
 */
bigint bigint_multiply(bigint i1, bigint i2);

/**
 * multiplies two specified ranges of integers using long (elementary) multiplication
 * @param i1 the first integer
 * @param i1i the index of the first digit in the first integer
 * @param i1f the index of the last digit (non-inclusive) in the first integer
 * @param i2 the second integer
 * @param i2i the index of the first digit in the second integer
 * @param i2f the index of the last digit (non-inclusive) in the second integer
 * @param outSize the variable to output the product size to
 * @return the integer array containing the product
 */
int *bigint_longMultiply(int *i1, unsigned int i1i, unsigned int i1f,
                         int *i2, unsigned int i2i, unsigned int i2f,
                         unsigned int *outSize);

/**
 * multiplies two specified ranges of integers using long (elementary) multiplication
 * @param i1 the first integer
 * @param i1size one more than the index of the last digit in the first integer
 * @param i2 the second integer
 * @param i2size one more than the index of the last digit in the second integer
 * @param idxi the index of the first digit in both integers, when padded to a power of 2 with zeros
 * @param idxf the index of the last digit in both integers, when padded to a power of 2 with zeros
 * @param outSize the variable to output the product size to
 * @return the integer array containing the product
 */
int *bigint_karatsubaMultiply(int *i1, unsigned int i1size,
                              int *i2, unsigned int i2size,
                              unsigned int idxi, unsigned int idxf,
                              unsigned int *outSize);

/**
 * left shift an integer array
 * @param i the integer
 * @param n the size of the array
 * @param d the number of times to left shift
 * @param newSize the location to store the size of the new array
 * @return the pointer to the reallocated array
 */
int *bigint_leftShiftArr(int *i, int n, int d, int *newSize);

/**
 * multiply an integer by the base
 * @param i the integer
 * @param d the number of times to multiply by the base
 * @return the result of the multiplication
 */
bigint bigint_baseMult(bigint i, int d);

/**
 * divide the integer by the base a number of times
 * @param i the integer
 * @param n the size of the array
 * @param d the number of times to divide by the base
 * @param newSize the location to store the size of the new array
 * @return the pointer to the reallocated array
 */
int *bigint_rightShiftArr(int *i, int n, int d, int *newSize);

/**
 * divide an integer by the base
 * @param i the integer
 * @param d the number of times to divide by the base
 * @return the result of the division
 */
bigint bigint_baseDivide(bigint i, int d);

#endif // BIGINT_H