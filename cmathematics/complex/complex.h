#include "../cmathematics.h"

#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
    bool inPolar;
    float r;
    float t;
} imag;

imag imag_rect(float r, float i);
imag imag_polar(float mag, float theta);

imag imag_add(imag i1, imag i2);
imag imag_sub(imag i1, imag i2);
imag imag_mul(imag i1, imag i2);
imag imag_div(imag i1, imag i2);

imag imag_addd(imag i, double d);
imag imag_subd(imag i, double d);
imag imag_muld(imag i, double d);
imag imag_divd(imag i, double d);

imag imag_pow(imag i1, int pow);

imag imag_conjugate(imag i);
float imag_magnitude(imag i);

imag imag_toPolar(imag i);
void imag_setPolar(imag *i);
imag imag_toRect(imag i);
void imag_setRect(imag *i);

char *imag_toString(imag i);
void imag_print(imag i);

#endif