#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    mat m = matrix(5, 5,
                   1.0, 3.0, 0.0, -5.0, 7.0,
                   1.0, 55.0, -2.3, 0.0, 3.0,
                   55.4, 0.0, 78.0, -14.0, 72.0,
                   222.0, 3.6, 4.0, 7.0, 0.0,
                   0.0, 1.0, 5.0, 3.0, 4.0);

    printMat(m);
    printf("%f\n", determinant(&m));
    printf("%f\n", determinantModREF(&m));
    freeMat(&m);

    return 0;
}