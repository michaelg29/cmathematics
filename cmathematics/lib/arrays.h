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

#endif // ARRAYS_H