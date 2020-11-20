#include <stdarg.h>
#include <stdio.h>

#include "cmathematics.h"

#ifndef VEC_H
#define VEC_H

/**
 * structure representing a vector
 */
typedef struct
{
    unsigned int dim; // the number of elements in the vector
    float *elements;  // pointer to the first element in the vector
} vec;

extern const vec VEC_UNDEFINED; // undefined vector (no dimension)

/**
 * allocate memory for a vector
 * @param dim the dimension of the vector
 * @return the vector
 */
vec allocate(unsigned int dim);

/**
 * construct a vector with a default value for all the values
 * @param dim the dimension of the vector
 * @param val the default value
 * @return the vector
 */
vec constructDefaultVector(unsigned int dim, float val);

/**
 * construct a vector with a default value of 0
 * @param dim the dimension of the vector
 * @return the vector
 */
vec constructEmptyVector(unsigned int dim);

/**
 * construct a new vector with specified values
 * <b>NOTE: must pass in floats as arguments</b>
 * @param dim the dimension of the vector
 * @param ... the values to put in the vector
 * @return the vector
 */
vec newVector(unsigned int dim, ...);

/**
 * construct a new vector with specified values
 * will automatically calculate the number of arguments
 * <b>NOTE: must pass in floats as arguments</b>
 * @param .. the values to put in the vector
 * @return the vector
 */
#define vector(...) newVector(NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

/**
 * copies a vector's values
 * @param v the vector to be copied
 * @return the copy (the parameter to the function)
 */
vec copy(vec v);

/**
 * copies a vector's values using memcpy
 * @param v the pointer to the vector to be copied
 * @return the copy
 */
vec copyPtr(vec *v);

/**
 * prints the values of a vector
 * @param v the vector
 */
void print(vec v);

/**
 * tests for equality between two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return true if the vectors have same dimension and equal corresponding values
 */
bool equals(vec v1, vec v2);

/**
 * adds a scalar to the values of a vector
 * @param v the vector
 * @param k the scalar
 * @return the resultant sum
 */
vec scalarAddition(vec v, float k);

/**
 * directly adds a scalar to the values of a vector
 * @param v the pointer to the vector
 * @param k the scalar
 */
void scalarAdditionTo(vec *v, float k);

/**
 * subtracts a scalar from the values of a vector
 * @param v the vector
 * @param k the scalar
 * @return the resultant sum
 */
vec scalarSubtraction(vec v, float k);

/**
 * directly subtracts a scalar from the values of a vector
 * @param v the pointer to the vector
 * @param k the scalar
 */
void scalarSubtractionFrom(vec *v, float k);

/**
 * multiplies the values of a vector by a scalar
 * @param v the vector
 * @param k the scalar
 * @return the multiplied vector
 */
vec scalarMultiplication(vec v, float k);

/**
 * directly multiplies the values of a vector by a scalar
 * @param v the pointer to the vector
 * @param k the scalar
 */
void scalarMultiplicationBy(vec *v, float k);

/**
 * divides the values of a vector by a scalar
 * @param v the vector
 * @param k the scalar
 * @return the divided vector
 */
vec scalarDivision(vec v, float k);

/**
 * directly divides the values of a vector by a scalar
 * @param v the pointer to the vector
 * @param k the scalar
 */
void scalarDivisionBy(vec *v, float k);

/**
 * raises the values of a vector to a scalar
 * @param v the vector
 * @param k the scalar
 * @return the resultant vector
 */
vec power(vec v, float k);

/**
 * directly raises the values of a vector to a power
 * @param v the pointer to the vector
 * @param k the power
 */
void powerOf(vec *v, float k);

/**
 * adds the corresponding elements in two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the sum of the vectors or VEC_UNDEFINED if the vectors have different dimensions
 */
vec add(vec v1, vec v2);

/**
 * add the coresponding elements of a vector to another vector
 * @param v1 the vector to be added to
 * @param v2 the vector to be added
 * @return true if the vectors have the same dimension
 */
bool addTo(vec *v1, vec v2);

/**
 * subtracts the corresponding elements in two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the difference of the vectors or VEC_UNDEFINED if the vectors have different dimensions
 */
vec subtract(vec v1, vec v2);

/**
 * subtracts the coresponding elements of a vector from another vector
 * @param v1 the vector to be subtracted from
 * @param v2 the vector to be subtracted
 * @return true if the vectors have the same dimension
 */
bool subtractFrom(vec *v1, vec v2);

/**
 * multiplies the corresponding elements in two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the element-wise multiplication of the vectors or VEC_UNDEFINED if the vectors have different dimensions
 */
vec multiply(vec v1, vec v2);

/**
 * multiplies the coresponding elements of a vector by another vector
 * @param v1 the vector to be multiplied by
 * @param v2 the vector to be multiplied
 * @return true if the vectors have the same dimension
 */
bool multiplyBy(vec *v1, vec v2);

/**
 * divides the corresponding elements in two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the element-wise division of the vectors or VEC_UNDEFINED if the vectors have different dimensions
 */
vec divide(vec v1, vec v2);

/**
 * divides the coresponding elements of a vector by another vector
 * @param v1 the vector to be divided by
 * @param v2 the vector to be divided
 * @return true if the vectors have the same dimension
 */
bool divideBy(vec *v1, vec v2);

/**
 * calculates the dot product of two vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the dot product of the vectors, 0 if they have different dimensions
 */
float dot(vec v1, vec v2);

/**
 * determines if two vectors are orthogonal
 * @param v1 the first vector
 * @param v2 the second vector
 * @return true if the vectors have the same dimension and the dot product is zero
 */
bool orthogonal(vec v1, vec v2);

/**
 * calculates the cross product of two vectors in three dimensions
 * cross product = vector perpendicular to both vectors
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the cross product of the two vectors, VEC_UNDEFINED if the vectors are not in 3D
 */
vec cross(vec v1, vec v2);

/**
 * calculates the magnitude of the vector
 * @param v the vector
 * @return the magnitude (square root of the sum of the squares of the components)
 */
float magnitude(vec v);

/**
 * calculates the square of the magnitude of the vector
 * @param v the vector
 * @return the magnitude squared (sum of the squares of the components)
 */
float magnitudeSquared(vec v);

/**
 * calculates the unit (normal) vector for a vector
 * normalized = divide each component by the magnitude
 * @param v the vector
 * @return the normal vector
 */
vec normalized(vec v);

/**
 * normalizes the vector
 * @param v the pointer to the vector to be normalized
 */
void normalize(vec *v);

#endif