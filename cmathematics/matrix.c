#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

const mat MAT_UNDEFINED = {0, 0, NULL}; // undefined vector (no dimension)

/**
 * allocate memory for a matrix
 * @param rows the number of rows
 * @param cols the number of cols
 * @return the matrix
 */
mat allocateMat(unsigned int rows, unsigned int cols)
{
    mat ret;

    ret.rows = rows;
    ret.cols = cols;

    // allocate the array of rows
    ret.elements = malloc(rows * sizeof(float));
    for (unsigned int i = 0; i < rows; i++)
    {
        // allocate the row's memory
        ret.elements[i] = malloc(cols * sizeof(float));
    }

    return ret;
}

/**
 * construct an identity matrix:
 * square matrix with ones along the main diagonal
 * @param dim the dimension of the matrix (the number of rows and cols)
 * @return the matrix
 */
mat identity(unsigned int dim)
{
    // allocate a square matrix
    mat ret = allocateMat(dim, dim);

    for (unsigned int r = 0; r < dim; r++)
    {
        for (unsigned int c = 0; c < dim; c++)
        {
            // if on the main diagonal, set to a 1, otherwise set to a 0
            ret.elements[r][c] = (r == c) ? 1.0f : 0.0f;
        }
    }

    return ret;
}

/**
 * construct a matrix with all zeros
 * @param rows the number of rows
 * @param cols the number of cols
 * @return the matrix
 */
mat zeroMatrix(unsigned int rows, unsigned int cols)
{
    mat ret = allocateMat(rows, cols);

    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            ret.elements[r][c] = 0.0f;
        }
    }

    return ret;
}

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
mat newMatrix(unsigned int rows, unsigned int cols, unsigned int numVals, ...)
{
    mat ret = allocateMat(rows, cols);

    va_list list;
    unsigned int size = rows * cols;
    va_start(list, size);

    unsigned int i = 0;

    for (unsigned int r = 0; r < rows; r++)
    {
        for (unsigned int c = 0; c < cols; c++)
        {
            if (i < numVals)
            {
                // get next element in list
                ret.elements[r][c] = va_arg(list, double);
            }
            else
            {
                // put a zero if no more elements passed in
                ret.elements[r][c] = 0.0f;
            }
        }
    }

    va_end(list);

    return ret;
}

/**
 * copies a matrix's values
 * @param m the matrix to be copied
 * @return the copy (the parameter to the function)
 */
mat copyMat(mat m)
{
    return m;
}

/**
 * copies a matrix's values using memcpy
 * @param m the pointer to the matrix to be copied
 * @return the copy
 */
mat copyMatPtr(mat *m)
{
    mat ret;
    memcpy(&ret, m, sizeof(mat));
    return ret;
}

/**
 * prints the values of a matrix
 * @param m the matrix
 */
void printMat(mat m)
{
    for (unsigned int r = 0; r < m.rows; r++)
    {
        printf("\n");

        printf("|");

        for (unsigned int c = 0; c < m.cols; c++)
        {
            printf(" %f", m.elements[r][c]);
        }

        printf(" |");
    }
    printf("\n");
}

/**
 * get the row of a matrix
 * @param m the pointer to the matrix
 * @param row the index of the row to be copied (count from 1)
 * @return the row as a vector
 */
vec getMatRow(mat *m, unsigned int row)
{
    row--; // subtract one because start counting from 1
    if (row < 0 || row >= m->rows)
    {
        // index out of bounds
        return VEC_UNDEFINED;
    }

    vec ret = allocateVec(m->cols);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        // get ith element in row
        ret.elements[i] = m->elements[row][i];
    }

    return ret;
}

/**
 * get the column of a matrix
 * @param m the pointer to the matrix
 * @param row the index of the column to be copied (count from 1)
 * @return the column as a vector
 */
vec getMatCol(mat *m, unsigned int col)
{
    col--; // subtract one because start counting from 1
    if (col < 0 || col >= m->cols)
    {
        // index out of bounds
        return VEC_UNDEFINED;
    }

    vec ret = allocateVec(m->rows);

    for (unsigned int i = 0; i < ret.dim; i++)
    {
        // get ith element in column
        ret.elements[i] = m->elements[i][col];
    }

    return ret;
}

/**
 * convert a vector to a single row matrix
 * @param v the pointer to the vector
 * @return the matrix with the vector as the first row
 */
mat toRowVec(vec *v)
{
    mat ret = allocateMat(1, v->dim); // 1 row, n columns

    for (unsigned int i = 0; i < v->dim; i++)
    {
        // put the values in the first row
        ret.elements[0][i] = v->elements[i];
    }

    return ret;
}

/**
 * convert a vector to a single column matrix
 * @param v the pointer to the vector
 * @return the matrix with the vector as the first column
 */
mat toColVec(vec *v)
{
    mat ret = allocateMat(v->dim, 1); // n rows, 1 column

    for (unsigned int i = 0; i < v->dim; i++)
    {
        // put the values in the first column
        ret.elements[i][0] = v->elements[i];
    }

    return ret;
}

/**
 * tests for equality between two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return true if the matrices have same dimension and equal corresponding values
 */
bool matEquals(mat m1, mat m2)
{
    if (m1.rows == m2.rows &&
        m1.cols == m2.cols)
    {
        // dimensions match
        for (unsigned int r = 0; r < m1.rows; r++)
        {
            for (unsigned int c = 0; c < m1.cols; c++)
            {
                if (m1.elements[r][c] != m2.elements[r][c])
                {
                    return false;
                }
            }
        }

        return true;
    }

    return false;
}

/**
 * adds a scalar to the values of a matrix
 * @param m the matrix
 * @param k the scalar
 * @return the resultant sum
 */
mat matScalarAddition(mat m, float k)
{
    mat ret = allocateMat(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m.elements[r][c] + k;
        }
    }

    return ret;
}

/**
 * directly adds a scalar to the values of a matrix
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarAdditionTo(mat *m, float k)
{
    for (unsigned int r = 0; r < m->rows; r++)
    {
        for (unsigned int c = 0; c < m->cols; c++)
        {
            m->elements[r][c] += k;
        }
    }
}

/**
 * subtracts a scalar from the values of a matrix
 * @param m the matrix
 * @param k the scalar
 * @return the resultant sum
 */
mat matScalarSubtraction(mat m, float k)
{
    mat ret = allocateMat(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m.elements[r][c] - k;
        }
    }

    return ret;
}

/**
 * directly subtracts a scalar from the values of a matrix
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarSubtractionFrom(mat *m, float k)
{
    for (unsigned int r = 0; r < m->rows; r++)
    {
        for (unsigned int c = 0; c < m->cols; c++)
        {
            m->elements[r][c] -= k;
        }
    }
}

/**
 * multiplies the values of a matrix by a scalar
 * @param m the matrix
 * @param k the scalar
 * @return the multiplied matrix
 */
mat matScalarMultiplication(mat m, float k)
{
    mat ret = allocateMat(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m.elements[r][c] * k;
        }
    }

    return ret;
}

/**
 * directly multiplies the values of a matrix by a scalar
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarMultiplicationBy(mat *m, float k)
{
    for (unsigned int r = 0; r < m->rows; r++)
    {
        for (unsigned int c = 0; c < m->cols; c++)
        {
            m->elements[r][c] *= k;
        }
    }
}

/**
 * divides the values of a matrix by a scalar
 * @param m the matrix
 * @param k the scalar
 * @return the divided matrix
 */
mat matScalarDivision(mat m, float k)
{
    mat ret = allocateMat(m.rows, m.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m.elements[r][c] / k;
        }
    }

    return ret;
}

/**
 * directly divides the values of a matrix by a scalar
 * @param m the pointer to the matrix
 * @param k the scalar
 */
void matScalarDivisionBy(mat *m, float k);
{
    for (unsigned int r = 0; r < m->rows; r++)
    {
        for (unsigned int c = 0; c < m->cols; c++)
        {
            m->elements[r][c] /= k;
        }
    }
}

/**
 * adds the corresponding elements in two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the sum of the matrices or MAT_UNDEFINED if hte matrices have different dimensions
 */
mat matAdd(mat m1, mat m2)
{
    if (m1.rows != m2.rows ||
        m1.cols != m2.cols)
    {
        // dimensions do not match
        return MAT_UNDEFINED;
    }

    mat ret = allocateMat(m1.rows, m1.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m1.elements[r][c] + m2.elements[r][c];
        }
    }

    return ret;
}

/**
 * add the corresponding elements of a matrix to another matrix
 * @param m1 the matrix to be added to
 * @param m2 the matrix to be added
 * @return true if the matrices have the same dimension
 */
bool matAddTo(mat *m1, mat m2)
{
    if (m1->rows != m2.rows ||
        m1->cols != m2.cols)
    {
        // dimensions do not match
        return false;
    }

    for (unsigned int r = 0; r < m1->rows; r++)
    {
        for (unsigned int c = 0; c < m1->cols; c++)
        {
            m1->elements[r][c] += m2.elements[r][c];
        }
    }

    return true;
}

/**
 * subtracts the corresponding elements in two matrices
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the difference of the matrices or MAT_UNDEFINED if hte matrices have different dimensions
 */
mat matSubtract(mat m1, mat m2)
{
    if (m1.rows != m2.rows ||
        m1.cols != m2.cols)
    {
        // dimensions do not match
        return MAT_UNDEFINED;
    }

    mat ret = allocateMat(m1.rows, m1.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            ret.elements[r][c] = m1.elements[r][c] - m2.elements[r][c];
        }
    }

    return ret;
}

/**
 * subtract the corresponding elements of a matrix from another matrix
 * @param m1 the matrix to be subtracted from
 * @param m2 the matrix to be subtracted
 * @return true if the matrices have the same dimension
 */
bool matSubtractFrom(mat *m1, mat m2)
{
    if (m1->rows != m2.rows ||
        m1->cols != m2.cols)
    {
        // dimensions do not match
        return false;
    }

    for (unsigned int r = 0; r < m1->rows; r++)
    {
        for (unsigned int c = 0; c < m1->cols; c++)
        {
            m1->elements[r][c] -= m2.elements[r][c];
        }
    }

    return true;
}

/**
 * multiplies a matrix by a vector
 * uses the row view of matrix-vector multiplication
 * @param m the matrix
 * @param v the vector
 * @return the vector product
 */
vec matVecMultiplication(mat m, vec v)
{
    if (m.cols != v.dim)
    {
        // number of columns in the matix must equal the number of elements of the vector
        return VEC_UNDEFINED;
    }

    vec ret = allocateVec(m.rows);

    for (unsigned int r = 0; r < ret.dim; r++)
    {
        // dot product of the vector with the corresponding row
        ret.elements[r] = dot(v, getMatRow(&m, r + 1));
    }

    return ret;
}

/**
 * multiplies a matrix by a matrix
 * @param m1 the first matrix
 * @param m2 the second matrix
 * @return the matrix product
 */
mat matMatMultiplication(mat m1, mat m2)
{
    if (m1.cols != m2.rows)
    {
        // number of columns in the first matix must equal the number of rows in the second matrix
        return MAT_UNDEFINED;
    }

    // create quick reference for the rows of the first matrix and columns of the second matrix
    vec *m1Rows = malloc(m1.rows * sizeof(vec));
    vec *m2Cols = malloc(m2.cols * sizeof(vec));

    for (unsigned int r = 0; r < m1.rows; r++)
    {
        m1Rows[r] = getMatRow(&m1, r + 1);
    }
    for (unsigned int c = 0; c < m2.cols; c++)
    {
        m2Cols[c] = getMatCol(&m2, c + 1);
    }

    mat ret = allocateMat(m1.rows, m2.cols);

    for (unsigned int r = 0; r < ret.rows; r++)
    {
        for (unsigned int c = 0; c < ret.cols; c++)
        {
            // dot product of the rth row of m1 with the cth column of m2
            ret.elements[r][c] = dot(m1Rows[r], m2Cols[c]);
        }
    }

    return ret;
}