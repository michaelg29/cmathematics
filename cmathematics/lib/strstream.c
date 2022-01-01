#include "strstream.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

strstream strstream_alloc(unsigned int size)
{
    strstream ret;
    ret.size = 0;
    ret.capacity = 0;

    // get sufficient capacity
    unsigned int capacity = 1;
    while (capacity < size)
    {
        capacity <<= 1;
    }

    // allocate
    ret.str = malloc(capacity * sizeof(char));
    if (ret.str)
    {
        ret.capacity = capacity;
        strstream_terminate(&ret);
    }

    return ret;
}

strstream strstream_allocDefault()
{
    return strstream_alloc(STRSTREAM_DEFAULT_SIZE);
}

strstream strstream_fromStr(char *str)
{
    int size = strlen(str);

    strstream ret = strstream_alloc(size);
    if (ret.capacity)
    {
        memcpy(ret.str, str, size * sizeof(char));
        ret.size = size;
    }

    return ret;
}

char strstream_realloc(strstream *s, unsigned int additionalLength)
{
    unsigned int newSize = s->size + additionalLength;

    if (newSize + 1 >= s->capacity)
    {
        // get required size
        unsigned int capacity = s->capacity;
        if (!capacity)
        {
            capacity = 1;
        }
        while (capacity <= newSize + 1)
        {
            capacity <<= 1;
        }

        // must reallocate
        char *oldMem = s->str;
        s->str = realloc(s->str, capacity * sizeof(char));
        if (!s->str)
        {
            // must allocate in new location
            s->str = malloc(capacity * sizeof(char));
            // copy existing characters
            memcpy(s->str, oldMem, s->capacity * sizeof(char));

            // update pointers
            free(oldMem);
        }

        s->capacity = capacity;
        return !0; // true
    }

    return 0; // false
}

/*
    accessors
*/
unsigned int strstream_available(strstream *s)
{
    return s->capacity - s->size - 1;
}

char *strstream_substrLength(strstream *s, unsigned int i, int n)
{
    if (n == 0 || i < 0 || i >= s->size || i + n < 0 || i + n > s->size)
    {
        return NULL;
    }

    unsigned int length = abs(n);

    char *ret = malloc((length + 1) * sizeof(char));
    if (n > 0)
    {
        // copy memory
        memcpy(ret, s->str + i, length);
    }
    else
    {
        // copy memory
        memcpy(ret, s->str + i - length, length);

        // reverse string if negative length
        for (unsigned int idx = 0; idx < length / 2; idx++)
        {
            unsigned int idx2 = length - idx - 1;

            ret[idx] ^= ret[idx2];
            ret[idx2] = ret[idx] ^ ret[idx2];
            ret[idx] = ret[idx] ^ ret[idx2];
        }
    }

    ret[length] = '\0';

    return ret;
}

char *strstream_substrRange(strstream *s, unsigned int i, unsigned int f)
{
    return strstream_substrLength(s, i, f - i);
}

void addStringToList(char ***list, int n, char *str, int strLen)
{
    if (strLen <= 0)
    {
        return;
    }

    char **tmp = malloc((n + 1) * sizeof(char *));
    memcpy(tmp, *list, n * sizeof(char *));
    free(*list);
    
    tmp[n] = malloc(strLen + 1);
    memcpy(tmp[n], str, strLen);
    tmp[n][strLen] = 0;
    
    *list = tmp;
}

int strstream_split(strstream *s, char regex, char ***out)
{
    char **ret = NULL;
    int n = 0;

    if (s && s->str && s->size)
    {
        unsigned int startIdx = 0;

        for (int i = 0; i < s->size; i++)
        {
            if (s->str[i] == regex)
            {
                int strLen = i - startIdx;
                if (!strLen)
                {
                    startIdx = i + 1;
                    continue;
                }

                // add string
                addStringToList(&ret, n, s->str + startIdx, strLen);
                n++;

                startIdx = i + 1;
            }
        }

        if (startIdx != s->size)
        {
            // add last string
            addStringToList(&ret, n, s->str + startIdx, s->size - startIdx);
            n++;
        }
    }

    *out = ret;

    return n;
}

void freeStringList(char **list, int n)
{
    if (list && n)
    {
        for (int i = 0; i < n; i++)
        {
            free(list[i]);
        }

        free(list);
    }
}

/*
    modifiers
*/
void strstream_terminate(strstream *s)
{
    if (s->str && s->capacity)
    {
        s->str[s->size] = '\0';
    }
}

void strstream_concat(strstream *s, const char *format, ...)
{
    va_list args;

    unsigned int available = strstream_available(s);
    va_start(args, format);
    int formatSize = vsnprintf(s->str + s->size, available * sizeof(char), format, args);
    va_end(args);

    if (strstream_realloc(s, formatSize))
    {
        // re-read in the formatted string to reallocated block
        available = strstream_available(s);
        va_start(args, format);
        formatSize = vsnprintf(s->str + s->size, available * sizeof(char), format, args);
        va_end(args);
    }

    s->size += formatSize;
    strstream_terminate(s);
}

void strstream_read(strstream *s, void *data, unsigned int length)
{
    strstream_realloc(s, length);

    // copy memory
    memcpy(s->str + s->size, data, length);

    s->size += length;
    strstream_terminate(s);
}

void strstream_retreat(strstream *s, unsigned int length)
{
    if (!s->size || !s->capacity)
    {
        // cannot retreat any further
        return;
    }

    // return to beginning if the length is too long
    // otherwise, move cursor back length positions
    s->size = length >= s->size ? 0 : s->size - length;
    // set terminator character
    strstream_terminate(s);
}

/*
    file IO
*/
void strstream_readFile(strstream *s, FILE *file, unsigned int length)
{
    // check if length == 0 => read to the end of the file
    if (!length)
    {
        // read rest of file, so need size
        int prev = ftell(file);
        fseek(file, 0L, SEEK_END); // move cursor to the end
        length = ftell(file) - prev;
        fseek(file, prev, SEEK_SET); // return to original cursor
    }

    strstream_realloc(s, length);

    // read from the file
    fread(s->str + s->size, sizeof(char), length, file);

    s->size += length;
    strstream_terminate(s);
}

void strstream_writeFile(strstream *s, FILE *file, unsigned int first, unsigned int last)
{
    // ensure first index in bounds
    if (first >= s->size)
    {
        return;
    }

    if (!last)
    {
        // write to end of the stream
        last = s->size;
    }

    // make sure last is after first
    if (last >= first)
    {
        // ensure last index in bounds
        if (last > s->size)
        {
            last = s->size;
        }

        fwrite(s->str + first, sizeof(char), last - first, file);
    }
}

/*
    clear
*/
void strstream_clear(strstream *s)
{
    free(s->str);
    s->size = 0;
    s->capacity = 0;
}