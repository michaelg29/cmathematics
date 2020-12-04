#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    mat m = matrix(4, 4,
                   1.0f, 3.0f, 1.0f, 4.0f,
                   3.0f, 9.0f, 5.0f, 15.0f,
                   6.0f, 2.0f, 1.0f, 1.0f,
                   12.0f, 4.0f, 2.0f, 3.0f);

    printMat(m);
    printf("%f\n", determinant(&m));

    return 0;
}