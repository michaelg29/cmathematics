#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"

int main()
{
    printf("Hello, world!\n");

    mat m = matrix(3, 4,
                   1.0f, 2.0f, 1.0f, 3.0f,
                   3.0f, -1.0f, -3.0f, -1.0f,
                   2.0f, 3.0f, 1.0f, 4.0f);
    printMat(m);
    printf("%f\n", determinant(m));
    rref(&m);
    printMat(m);

    return 0;
}