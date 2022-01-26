#include "numio.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../cmathematics.h"
#include "../lib/strstream.h"

// array containing hexadecimal digits in order
char hex[16] = "0123456789ABCDEF";

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
    if (!noBytes)
    {
        return NULL;
    }

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

void printHexString(char *array, int n, const char *title)
{
    unsigned char *tmp = printByteArr(array, n, 0, 0, 0);
    printf("%s: %s\n", title, tmp);
    free(tmp);
}

unsigned int smallEndianValue(unsigned char *str, int n)
{
    n = MIN(n, sizeof(unsigned int));

    unsigned int ret = 0;

    while (n--) {
        ret <<= 8;
        ret |= str[n];
    }

    return ret;
}

char *smallEndianStr(unsigned int val)
{
    char *ret = malloc(sizeof(unsigned int));
    memset(ret, 0, sizeof(unsigned int));

    for (int i = 0; i < sizeof(unsigned int); i++)
    {
        ret[i] = (unsigned char)val;
        val >>= 8;
    }

    return ret;
}

unsigned int largeEndianValue(unsigned char *str, int n)
{
    n = MIN(n, sizeof(unsigned int));

    unsigned int ret = 0;

    for (int i = 0; i < n; i++)
    {
        ret <<= 8;
        ret |= str[i];
    }

    return ret;
}

char *largeEndianStr(unsigned int val)
{
    char *ret = malloc(sizeof(unsigned int));
    memset(ret, 0, sizeof(unsigned int));

    // <= because want last character to be 0
    for (int i = sizeof(unsigned int) - 1; i >= 0; i--)
    {
        ret[i] = (unsigned char)val;
        val >>= 8;
    }

    return ret;
}