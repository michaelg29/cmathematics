#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"

int main()
{
    printf("Hello, world!\n");

    mat m1 = newMatrix(2, 3,
                       2.0f, 1.0f, 4.0f,
                       0.0f, 1.0f, 1.0f);
    printMat(m1);

    mat m2 = newMatrix(3, 4,
                       6.0f, 3.0f, -1.0f, 0.0f,
                       1.0f, 1.0f, 0.0, 4.0f,
                       -2.0f, 5.0f, 0.0f, 2.0f);
    printMat(m2);

    printMat(matMatMultiplication(m1, m2));

    return 0;
}