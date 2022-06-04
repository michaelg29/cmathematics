#include "complex.h"
#include "../lib/dynamicarray.h"

#ifndef COMPLEX_FUNCTIONS_H
#define COMPLEX_FUNCTIONS_H

// a z^k
typedef struct {
    int deg;
    imag coefficient;
} imag_polynomial_term;

// a (z - z0)^k
typedef struct {
    int deg;
    imag coefficient;
    imag z0;
} imag_binomial_term;

typedef struct {
    bool binomial_form;
    dynamicarray terms;
} imag_polynomial;

typedef struct {
    imag_polynomial numerator;
    imag_polynomial denominator;
} imag_rational;

imag_polynomial imag_polynomial_new(bool binomial_form, int numTerms, ...);
imag_rational imag_rational_new(imag_polynomial numerator, imag_polynomial denominator);
imag_rational imag_rational_new_terms(bool numerator_binomial, int numerator_numTerms, bool denominator_binomial, int denominator_numTerms);

void imag_polynomial_free(imag_polynomial *p);
void imag_rational_free(imag_rational *r);

imag imag_exp(imag i);
imag imag_sin(imag i);
imag imag_cos(imag i);
imag imag_tan(imag i);
imag imag_log(imag i);
imag imag_pow(imag base, imag pow);

#endif // COMPLEX_FUNCTIONS_H