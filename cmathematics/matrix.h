#include <stdarg.h>
#include <stdio.h>

#include "cmathematics.h"
#include "vec.h"

#ifndef MATRIX_H
#define MATRIX_H

/**
 * structure representing a matrix
 */
typedef struct
{
    unsigned int rows; // the number of rows in the matrix
    unsigned int cols; // the number of columns in the matrix
    float **elements;  // pointer to the first row of the matrix (array)
} mat;

extern const mat MAT_UNDEFINED; // undefined vector (no dimension)

/**
 * allocate memory for a matrix
 * @param rows the number of rows
 * @param cols the number of cols
 * @return the matrix
 */
mat allocateMat(unsigned int rows, unsigned int cols);

/**
 * construct an identity matrix:
 * square matrix with ones along the main diagonal
 * @param dim the dimension of the matrix (the number of rows and cols)
 * @return the matrix
 */
mat identity(unsigned int dim);

/**
 * construct a matrix with all zeros
 * @param rows the number of rows
 * @param cols the number of cols
 * @return the matrix
 */
mat zeroMatrix(unsigned int rows, unsigned int cols);

/**
 * construct a new matrix with specified values
 * <b>NOTE: must pass in floats as arguments</b>
 * <b>NOTE: pass values in row by row</b>
 * @param rows the number of rows
 * @param cols the number of cols
 * @param numVals the number of values passed in
 * @param ... the values to put in the matrix
 * @return the matrix
 */
mat newMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...);

/**
 * construct a new matrix with specified values
 * will automatically calculate the number of arguments
 * <b>NOTE: must pass in floats as arguments</b>
 * @param rows the number of rows
 * @param cols the number of cols
 * @param .. the values to put in the matrix
 * @return the matrix
 */
#define matrix(unsigned int rows, unsigned int cols, ...) newMatrix(rows, cols, NUMARGS(float, __VA_ARGS__), ##__VA_ARGS__)

/**
 * copies a matrix's values
 * @param m the matrix to be copied
 * @return the copy (the parameter to the function)
 */
mat copyMat(mat m);

/**
 * copies a matrix's values using memcpy
 * @param m the pointer to the matrix to be copied
 * @return the copy
 */
mat copyMatPtr(mat *m);

/**
 * prints the values of a matrix
 * @param m the matrix
 */
void printMat(mat m);

/**
 * get the row of a matrix
 * @param m the pointer to the matrix
 * @param row the index of the row to be copied (count from 1)
 * @return the row as a vector
 */
vec getMatRow(mat *m, unsigned int row);

/**
 * get the column of a matrix
 * @param m the pointer to the matrix
 * @param row the index of the column to be copied (count from 1)
 * @return the column as a vector
 */
vec getMatCol(mat *m, unsigned int col);

/**
 * convert a vector to a single row matrix
 * @param v the pointer to the vector
 * @return the matrix with the vector as the first row
 */
mat toRowVec(vec *v);

/**
 * convert a vector to a single column matrix
 * @param v the pointer to the vector
 * @return the matrix with the vector as the first column
 */
mat toColVec(vec *v);

/**
 * tests for equality between two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return true if the matrices have same dimension and equal corresponding values
 */
bool matEquals(mat m1, mat m2);

/**
 * adds a scalar to the values of a matrix
 * @param m the matrix
 * @param k the scalar
 * @return the resultant sum
 */
mat matScalarAddition(mat m, float k);

/**
 * directly adds a scalar to the values of a matrix
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarAdditionTo(mat *m, float k);

/**
 * subtracts a scalar from the values of a matrix
 * @param m the matrix
 * @param k the scalar
 * @return the resultant sum
 */
mat matScalarSubtraction(mat m, float k);

/**
 * directly subtracts a scalar from the values of a matrix
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarSubtractionFrom(mat *m, float k);

/**
 * multiplies the values of a matrix by a scalar
 * @param m the matrix
 * @param k the scalar
 * @return the multiplied matrix
 */
mat matScalarMultiplication(mat m, float k);

/**
 * directly multiplies the values of a matrix by a scalar
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarMultiplicationBy(mat *m, float k);

/**
 * divides the values of a matrix by a scalar
 * @param m the matrix
 * @param k the scalar
 * @return the divided matrix
 */
mat matScalarDivision(mat m, float k);

/**
 * directly divides the values of a matrix by a scalar
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarDivisionBy(mat *m, float k);

/**
 * adds the corresponding elements in two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the sum of the matrices or MAT_UNDEFINED if hte matrices have different dimensions
 */
mat matAdd(mat m1, mat m2);

/**
 * add the corresponding elements of a matrix to another matrix
 * @param m1 the matrix to be added to
 * @param m2 the matrix to be added
 * @return true if the matrices have the same dimension
 */
bool matAddTo(mat *m1, mat m2);

/**
 * subtracts the corresponding elements in two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the difference of the matrices or MAT_UNDEFINED if hte matrices have different dimensions
 */
mat matSubtract(mat m1, mat m2);

/**
 * subtract the corresponding elements of a matrix from another matrix
 * @param m1 the matrix to be subtracted from
 * @param m2 the matrix to be subtracted
 * @return true if the matrices have the same dimension
 */
bool matSubtractFrom(mat *m1, mat m2);

/**
 * multiplies a matrix by a vector
 * uses the row view of matrix-vector multiplication
 * @param m the matrix
 * @param v the vector
 * @return the vector product
 */
vec matVecMultiplication(mat m, vec v);

/**
 * multiplies a matrix by a matrix
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the matrix product
 */
mat matMatMultiplication(mat m1, mat m2);

#endif