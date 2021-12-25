#include <stdio.h>
#include <stdlib.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/util/bigint.h"

int main() {
    bigint i1 = bigint_fromString("134978536589136051298354084375028347509814357120834436128754692635894326592735617623458126348943987324562");
    char *i1_str = bigint_toString(i1);
    bigint i2 = bigint_fromString("5461345789182374071659126451672354893245629374651635498132675618723456761823694817235617463519236468712");
    char *i2_str = bigint_toString(i2);

    bigint add = bigint_add(i1, i2);
    char *add_str = bigint_toString(add);
    bigint sub = bigint_subtract(i1, i2);
    char *sub_str = bigint_toString(sub);
    bigint mult = bigint_multiply(i1, i2);
    char *mult_str = bigint_toString(mult);
    
    printf("i1       = (%d) %s\n", i1.noDigits, i1_str);
    printf("i2       = (%d) %s\n", i2.noDigits, i2_str);
    printf("i1 + i2  = (%d) %s\n", add.noDigits, add_str);
    printf("i1 - i2  = (%d) %s\n", sub.noDigits, sub_str);
    printf("i1 * i2  = (%d) %s\n", mult.noDigits, mult_str);

    free(i1_str);
    free(i2_str);
    free(add_str);
    free(sub_str);
    free(mult_str);

    i1 = bigint_baseMult(i1, 2);
    i1_str = bigint_toString(i1);
    printf("i1 * b^%d = (%d) %s\n", 2, i1.noDigits, i1_str);
    free(i1_str);

    i2 = bigint_baseDivide(i2, 2);
    i2_str = bigint_toString(i2);
    printf("i2 / b^%d = (%d) %s\n", 2, i2.noDigits, i2_str);
    free(i2_str);

    bigint_free(&i1);
    bigint_free(&i2);
    bigint_free(&add);
    bigint_free(&sub);
    bigint_free(&mult);
}