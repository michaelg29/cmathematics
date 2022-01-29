#include "complex.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "../trig/trig.h"

#include "../lib/strstream.h"

imag imag_rect(float r, float i)
{
    imag ret = {false, r, i};
    return ret;
}

imag imag_polar(float mag, float theta)
{
    imag ret = {true, mag, theta};
    return ret;
}

imag imag_add(imag i1, imag i2)
{
    imag ret = {false, 0.0f, 0.0f};

    bool setPolar = i1.inPolar && i2.inPolar;

    if (i1.inPolar)
    {
        i1 = imag_toRect(i1);
    }
    ret.r += i1.r;
    ret.t += i1.t;

    if (i2.inPolar)
    {
        i2 = imag_toRect(i2);
    }
    ret.r += i2.r;
    ret.t += i2.t;

    if (setPolar) {
        imag_setPolar(&ret);
    }

    return ret;
}

imag imag_sub(imag i1, imag i2)
{
    imag ret = {false, 0.0f, 0.0f};

    bool setPolar = i1.inPolar && i2.inPolar;

    if (i1.inPolar)
    {
        imag_setRect(&i1);
    }
    ret.r += i1.r;
    ret.t += i1.t;

    if (i2.inPolar)
    {
        imag_setRect(&i2);
    }
    ret.r += i2.r;
    ret.t += i2.t;

    if (setPolar) {
        imag_setPolar(&ret);
    }

    return ret;
}

imag imag_mul(imag i1, imag i2)
{
    imag ret = {true, 0.0f, 0.0f};

    if (!(i1.inPolar || i2.inPolar))
    {
        ret.r = i1.r * i2.r - i1.t * i2.t;
        ret.t = i1.r * i2.t + i1.t * i2.r;
        ret.inPolar = false;
    }
    else
    {
        if (!i1.inPolar)
        {
            imag_setPolar(&i1);
        }
        if (!i2.inPolar)
        {
            imag_setPolar(&i2);
        }

        ret.r = i1.r * i2.r;
        ret.t = i1.t + i2.t;
    }

    return ret;
}

imag imag_div(imag i1, imag i2)
{
    imag ret = {true, 0.0f, 0.0f};

    if (!(i1.inPolar || i2.inPolar))
    {
        float denominator = imag_magnitude(i2);

        ret.r = (i1.r * i2.r + i1.t * i2.t) / denominator;
        ret.t = (i1.t * i2.r - i1.r * i2.t) / denominator;
        ret.inPolar = false;
    }
    else
    {
        if (!i1.inPolar)
        {
            imag_setPolar(&i1);
        }
        if (!i2.inPolar)
        {
            imag_setPolar(&i2);
        }

        ret.r = i1.r * i2.r;
        ret.t = i1.t - i2.t;
    }

    return ret;
}

imag imag_pow(imag i, int pow)
{
    imag ret = i;

    if (!ret.inPolar)
    {
        imag_setPolar(&ret);
    }

    ret.r = powf(ret.r, (float)pow);
    ret.t *= pow;

    if (!i.inPolar)
    {
        imag_setRect(&ret);
    }

    return ret;
}

imag imag_conjugate(imag i)
{
    imag ret = i;
    ret.t *= -1;
    return ret;
}

float imag_magnitude(imag i)
{
    if (i.inPolar)
    {
        return i.r;
    }

    return sqrtf(i.r * i.r + i.t * i.t);
}

imag imag_toPolar(imag i)
{
    imag ret = i;

    imag_setPolar(&ret);

    return ret;
}

void imag_setPolar(imag *i)
{
    if (i->inPolar)
    {
        return;
    }

    float r = imag_magnitude(*i);
    float t = invtan(i->r, i->t);

    i->r = r;
    i->t = t;
    i->inPolar = true;
}

imag imag_toRect(imag i)
{
    imag ret = i;

    imag_setRect(&ret);

    return ret;
}

void imag_setRect(imag *i)
{
    if (!i->inPolar)
    {
        return;
    }

    float r = i->r * cosf(i->t);
    float t = i->r * sinf(i->t);

    i->r = r;
    i->t = t;
    i->inPolar = false;
}

char *imag_toString(imag i)
{
    strstream ret = strstream_allocDefault();

    if (i.inPolar)
    {
        strstream_concat(&ret, "%fang(%f)", i.r, i.t);
    }
    else
    {
        strstream_concat(&ret, "%f + i%f", i.r, i.t);
    }
    
    return ret.str;
}

void imag_print(imag i)
{
    char *str = imag_toString(i);
    printf("%s\n", str);
    free(str);
}