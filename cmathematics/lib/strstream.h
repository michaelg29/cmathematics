#include <stdio.h>

#ifndef STRSTREAM_H
#define STRSTREAM_H

#define STRSTREAM_DEFAULT_SIZE (1 << 2)

typedef struct strstream
{
    char *str;
    unsigned int capacity;
    unsigned int size;
} strstream;

/*
    allocation
*/
strstream strstream_alloc(unsigned int size);
strstream strstream_allocDefault();
strstream strstream_fromStr(char *str);
char strstream_realloc(strstream *s, unsigned int additionalLength);

/*
    accessors
*/
unsigned int strstream_available(strstream *s);
char *strstream_substrLength(strstream *s, unsigned int i, int n);
char *strstream_substrRange(strstream *s, unsigned int i, unsigned int f);

int strstream_split(strstream *s, char regex, char ***out);
void freeStringList(char **list, int n);

unsigned int strstream_indexOf(strstream *s, char c, int initIdx);

/*
    modifiers
*/
void strstream_terminate(strstream *s);
void strstream_concat(strstream *s, const char *format, ...);
void strstream_read(strstream *s, void *data, unsigned int length);
void strstream_retreat(strstream *s, unsigned int length);

/*
    file IO
*/
void strstream_readFile(strstream *s, FILE *file, unsigned int length);
void strstream_writeFile(strstream *s, FILE *file, unsigned int first, unsigned int last);

/*
    clear
*/
void strstream_clear(strstream *s);

#endif