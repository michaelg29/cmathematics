#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    mat m = matrix(3, 3,
                   1.0, 2.0, 3.0,
                   0.0, 4.0, 5.0,
                   1.0, 0.0, 6.0);

    printMat(m);

    mat inv = inverseMat(&m);
    printMat(matScalarMultiplication(inv, 22.0f));

    printMat(matScalarMultiplication(inverseMatRREF(&m), 22.0f));

    return 0;
}