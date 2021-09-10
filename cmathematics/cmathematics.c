#include "cmathematics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "lib/strstream.h"

// array containing hexadecimal digits in order
char hex[16] = "0123456789ABCDEF";

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
 * method to reverse an array between two bounds
 * @param arr the array
 * @param i the left bound
 * @param f the right bound
 */
void reverseArray(unsigned char *arr, int i1, int i2)
{
    while (i1 < i2)
    {
        // swap corresponding elements across the middle
        unsigned char tmp = arr[i1];
        arr[i1] = arr[i2];
        arr[i2] = tmp;
        i1++;
        i2--;
    }
}

/**
 * method to rotate an array to the left using the reversal algorithm
 * @param arr the array
 * @param d the number of positions to shift
 * @param n the length of the array
 */
void leftRotate(unsigned char *arr, int d, int n)
{
    reverseArray(arr, 0, d - 1);
    reverseArray(arr, d, n - 1);
    reverseArray(arr, 0, n - 1);
}

/**
 * method to rotate an array to the right using the reversal algorithm
 * @param arr the array
 * @param d the number of positions to shift
 * @param n the length of the array
 */
void rightRotate(unsigned char *arr, int d, int n)
{
    // complementary left rotation
    leftRotate(arr, n - d, n);
}

void randomCharArray(unsigned char *arr, int n)
{
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() & 0xff; // cap at 255
    }
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

/**
 * method to scan a string in hexadecimal format and return its byte array representation
 * @param str the hexadecimal string
 * @param bytes the number of bytes in the output
 * @return the byte array with the hexadecimal data
 */
unsigned char *scanHex(char *str, int bytes) {
    unsigned char *ret = malloc(bytes);
    memset(ret, 0, bytes);

    for (int i = 0, i2 = 0; i < bytes; i++, i2 += 2) {
        // get value
        for (int j = 0; j < 2; j++) {
            ret[i] <<= 4;
            unsigned char c = str[i2 + j];
            if (c >= '0' && c <= '9') {
                ret[i] += c - '0';
            }
            else if (c >= 'a' && c <= 'f') {
                ret[i] += c - 'a' + 10;
            }
            else if (c >= 'A' && c <= 'F') {
                ret[i] += c - 'A' + 10;
            }
            else {
                free(ret);
                return NULL;
            }
        }
    }

    return ret;
}

/**
 * method to output a byte array as a hexadecimal string
 * @param arr the byte array
 * @param noBytes the number of bytes
 * @param delim word delimiter
 * @param delimN the size of the delimiter
 * @param wordSize number of bytes in a word
 * @return the formatted string
 */
unsigned char *printByteArr(unsigned char *arr, int noBytes, unsigned char *delim, int delimN, int wordSize)
{
    if (!wordSize)
    {
        wordSize = noBytes;
    }

    int outLen = 0;
    int noWords = noBytes / wordSize;
    int extra = noBytes % wordSize;

    outLen += noWords * (2 * wordSize + delimN); // each complete word gets 2 chars per byte and a delimeter
    outLen += extra * 2; // extra characters

    strstream ret = strstream_alloc(outLen);

    // format delimiter as a string with a temrinator character
    unsigned char *delimStr = NULL;
    if (delimN)
    {
        delimStr = malloc(delimN + 1);
        if (!delimStr)
        {
            return NULL;
        }
        memcpy(delimStr, delim, delimN);
        delimStr[delimN] = '\0';
    }

    int wordIdx = 0;

    for (int i = 0; i < noBytes; i++)
    {
        strstream_concat(&ret, "%c%c", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
        wordIdx++;
        if (delimN && wordIdx == wordSize)
        {
            // add delimiter
            strstream_concat(&ret, "%s", delimStr);
            wordIdx = 0;
        }
    }

    if (!extra)
    {
        // remove final delimiter
        strstream_retreat(&ret, delimN);
    }

    return ret.str;
}