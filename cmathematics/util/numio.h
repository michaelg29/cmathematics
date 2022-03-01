#ifndef NUMIO_H
#define NUMIO_H

// array containing hexadecimal digits in order
extern char hex[16];

/**
 * method to scan a string in hexadecimal format and return its byte array representation
 * @param str the hexadecimal string
 * @param bytes the number of bytes in the output
 * @return the byte array with the hexadecimal data
 */
unsigned char *scanHex(char *str, int bytes);

/**
 * method to output a byte array as a hexadecimal string
 * @param arr the byte array
 * @param noBytes the number of bytes
 * @param delim word delimiter
 * @param delimN the size of the delimiter
 * @param wordSize number of bytes in a word
 * @return the formatted string
 */
unsigned char *printByteArr(unsigned char *arr, int noBytes, unsigned char *delim, int delimN, int wordSize);

unsigned int smallEndianValue(unsigned char *str, int n);
char *newSmallEndianStr(unsigned int val);
void smallEndianStr(unsigned int val, unsigned char *out, int n);

unsigned int largeEndianValue(unsigned char *str, int n);
char *newLargeEndianStr(unsigned int val);
void largeEndianStr(unsigned int val, unsigned char *out, int n);

#endif // NUMIO_H