#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/trig/trig.h"
#include "../cmathematics/complex/complex.h"

int main() {
    imag i1 = imag_rect(3.0f, 1.0f);
    imag_print(i1);
    i1 = imag_toPolar(i1);
    imag_print(i1);
    imag_setRect(&i1);
    imag_print(i1);

    imag i2 = imag_polar(3.0f, M_PI);
    imag_print(i2);
    i2 = imag_toRect(i2);
    imag_print(i2);
    imag_setPolar(&i2);
    imag_print(i2);

    return 0;
}