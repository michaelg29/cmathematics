#include "numio.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

char *newSmallEndianStr(unsigned int val)
{
    char *ret = malloc(sizeof(unsigned int));
    memset(ret, 0, sizeof(unsigned int));

    smallEndianStr(val, ret, sizeof(unsigned int));
    
    return ret;
}

void smallEndianStr(unsigned int val, unsigned char *out, int n)
{
    // write entire 4 byte number or until end of string
    for (int i = 0; i < sizeof(unsigned int) && i < n; i++)
    {
        out[i] = (unsigned char)val;
        val >>= 8;
    }
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

char *newLargeEndianStr(unsigned int val)
{
    char *ret = malloc(sizeof(unsigned int));
    memset(ret, 0, sizeof(unsigned int));

    largeEndianStr(val, ret, sizeof(unsigned int));

    return ret;
}

void largeEndianStr(unsigned int val, unsigned char *out, int n)
{
    // write entire number or size of string
    for (int i = MIN(n, sizeof(unsigned int)) - 1; i >= 0; i--)
    {
        out[i] = (unsigned char)val;
        val >>= 8;
    }
}

char padding = '=';
char base64Digits[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char *base64_encode(unsigned char *str, unsigned int strLen)
{
    unsigned int outLen = ceil((double)strLen * 8.0 / 6.0);
    outLen += 4 - outLen & 0x3;
    unsigned char *ret = malloc(outLen + 1);

    unsigned int inIdx = 0;
    unsigned int outIdx = 0;
    for (unsigned int bitPos = 0, maxBitPos = strLen << 3; bitPos < maxBitPos; bitPos += 6, outIdx++)
    {
        // take 6 bits starting at bitPos
        unsigned int val = 0;

        // get bits from current byte
        unsigned int startIdx = bitPos & 0x7; // bitPos % 8

        // read two bytes
        short combo = (str[inIdx] << 8) | str[inIdx + 1];

        // take 6 bits starting from startIdx
        val = (combo >> (16 - 6 - startIdx)) & 0b111111;

        if (startIdx >= 2)
        {
            // next group starts on next character
            inIdx++;
        }

        ret[outIdx] = base64Digits[val];
    }

    // pad with '='
    memset(ret + outIdx, padding, outLen - outIdx);
    ret[outLen] = 0;

    return ret;
}

unsigned char *base64_decode(unsigned char *str, unsigned int strLen)
{
    unsigned char *ret = malloc(strLen);
    unsigned int outIdx = 0;
    
    for (unsigned int i = 0; i < strLen; i += 4)
    {
        unsigned int val = 0;
        for (int j = 0; j < 4; j++)
        {
            val <<= 6;
            if (i + j < strLen)
            {
                // to get number from base 64 character
                unsigned char c = str[i + j];
                printf("%c ", c);
                if (c >= 'a')
                {
                    c = c - 'a' + 26;
                }
                else if (c >= 'A')
                {
                    c = c - 'A';
                }
                else if (c >= '0')
                {
                    c = c - '0' + 52;
                }
                else if (c == '+')
                {
                    c = 62;
                }
                else if (c == '/')
                {
                    c = 63;
                }
                else if (c == padding)
                {
                    c = 0;
                }
                else
                {
                    free(ret);
                    return NULL;
                }

                val |= c;
                printf("%d; ", c);
            }
        }

        printf("=> %d, %08x\n", val, val);
        memcpy(ret + outIdx, &val, 3);
        unsigned char c = ret[outIdx];
        ret[outIdx] = ret[outIdx + 2];
        ret[outIdx + 2] = c;
        outIdx += 3;
    }

    ret[outIdx] = 0;

    return ret;
}