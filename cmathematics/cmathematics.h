#ifndef CMATHEMATICS_H
#define CMATHEMATICS_H

typedef unsigned char bool;
#define false 0
#define true !false

/**
 * calculates the number of arguments in a __VA_ARGS__ list
 * @param type the type of the argument in the list
 * @param ... the list
 * @return the number of arguments in the list
 */
#define NUMARGS(type, ...) (sizeof((type[]){0.0f, ##__VA_ARGS__}) / sizeof(type) - 1)

/**
 * finds the maximum of two values
 * @param i1 the first value
 * @param i2 the second value
 * @return the larger element
 */
#define MAX(i1, i2) (i1 > i2 ? i1 : i2)

/**
 * method to determine if an integer exists in an array
 * @param arr the array
 * @param noElements the number of elements in the array
 * @param target the target element
 * @return if the element exists in the array
 */
bool containsUint(unsigned int *arr, unsigned int noElements, unsigned int target);

/**
 * method to print out an integer array
 * @param arr the array
 * @param noElements the number of elements in the array
 */
void printUintArray(unsigned int *arr, unsigned int noElements);

#endif