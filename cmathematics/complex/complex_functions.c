#include "complex.h"
#include "complex_functions.h"
#include "math.h"
#include "../trig/trig.h"

#include "../lib/dynamicarray.h"

#include <stdio.h>
#include <stdarg.h>

imag_polynomial imag_polynomial_new(bool binomial_form, int numTerms, ...)
{
    // allocate
    imag_polynomial ret;
    ret.binomial_form = binomial_form;
    ret.terms = dynarr_allocate(numTerms);

    // get terms
    va_list terms;
    va_start(terms, numTerms);

    for (int i = 0; i < numTerms; i++)
    {
        void *term = NULL;
        if (binomial_form)
        {
            imag_binomial_term *bterm = malloc(sizeof(imag_binomial_term));
            *bterm = va_arg(terms, imag_binomial_term);
            term = bterm;
        }
        else
        {
            imag_polynomial_term *pterm = malloc(sizeof(imag_polynomial_term));
            *pterm = va_arg(terms, imag_polynomial_term);
            term = pterm;
        }
        dynarr_addLast(&ret.terms, term);
    }

    return ret;
}

imag_rational imag_rational_new(imag_polynomial numerator, imag_polynomial denominator);
imag_rational imag_rational_new_terms(bool numerator_binomial, int numerator_numTerms, bool denominator_binomial, int denominator_numTerms);

void imag_polynomial_free(imag_polynomial *p)
{
    dynarr_freeDeep(&p->terms);
    dynarr_clear(&p->terms);
}

void imag_rational_free(imag_rational *r)
{
    imag_polynomial_free(&r->numerator);
    imag_polynomial_free(&r->denominator);
}

imag imag_exp(imag i)
{
    imag ret = i;
    imag_setRect(&i);
    if (ret.inPolar)
    {
        ret.r = expf(i.r);
        ret.t = invtan(i.r, i.t);
    }
    else
    {
        ret.r = expf(i.r) * cosf(i.t);
        ret.t = expf(i.r) * sinf(i.t);
    }
}

imag imag_sin(imag i)
{
    imag_setRect(&i);
    i = imag_mul(i, imag_rect(0.0f, 1.0f));
    imag i2 = imag_muld(i, -1.0);
    
    return imag_div(imag_sub(imag_exp(i), imag_exp(i2)), imag_rect(0.0f, 2.0f));
}

imag imag_cos(imag i)
{
    imag_setRect(&i);
    i = imag_mul(i, imag_rect(0.0f, 1.0f));
    imag i2 = imag_muld(i, -1.0);
    
    return imag_divd(imag_add(imag_exp(i), imag_exp(i2)), 2.0);
}

imag imag_tan(imag i)
{
    imag_setRect(&i);
    i = imag_mul(i, imag_rect(0.0f, 1.0f));
    imag i2 = imag_muld(i, -1.0);

    return imag_div(imag_sub(imag_exp(i), imag_exp(i2)), imag_add(imag_exp(i), imag_exp(i2)));
}

imag imag_log(imag i)
{
    imag_setPolar(&i);

    return imag_rect(logf(i.r), i.t);
}

imag imag_pow(imag base, imag pow)
{
    return imag_exp(imag_mul(pow, imag_log(base)));
}