#ifndef ARRAYS_H
#define ARRAYS_H

/**
 * method to determine if an integer exists in an array
 * @param arr the array
 * @param noElements the number of elements in the array
 * @param target the target element
 * @return if the element exists in the array
 */
char arrContains(void **arr, int n, void *target);

// random array generation
char *newRandomBytes(int n);
void randomBytes(char *out, int n);

/**
 * method to reverse an array between two bounds
 * @param arr the array
 * @param i the left bound
 * @param f the right bound
 */
void reverseArray(unsigned char *arr, int i1, int i2);

/**
 * method to rotate an array to the left using the reversal algorithm
 * @param arr the array
 * @param d the number of positions to shift
 * @param n the length of the array
 */
void leftRotate(unsigned char *arr, int d, int n);

unsigned long long leftRotateLL(unsigned long long w, unsigned int d);
unsigned int leftRotateI(unsigned int w, unsigned int d);
unsigned char leftRotateC(unsigned char w, unsigned int d);

/**
 * method to rotate an array to the right using the reversal algorithm
 * @param arr the array
 * @param d the number of positions to shift
 * @param n the length of the array
 */
void rightRotate(unsigned char *arr, int d, int n);

unsigned long long rightRotateLL(unsigned long long w, unsigned int d);
unsigned int rightRotateI(unsigned int w, unsigned int d);
unsigned char rightRotateC(unsigned char w, unsigned int d);

#endif // ARRAYS_H