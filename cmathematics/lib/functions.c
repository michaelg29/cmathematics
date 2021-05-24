#include <math.h>
#include <float.h>

#include "functions.h"

double csc(double x)
{
    return 1.0 / sin(x);
}

double sec(double x)
{
    return 1.0 / cos(x);
}

double cot(double x)
{
    return cos(x) / sin(x);
}

double acsc(double x)
{
    return x == 0.0 ? nan("0xF") : asin(1.0 / x);
}

double asec(double x)
{
    return x == 0.0 ? DBL_MAX : acos(1.0 / x);
}

double acot(double x)
{
    return x == 0.0 ? M_PI / 2 : atan(1.0 / x);
}

double absf(double x)
{
    return x < 0.0 ? -x : x;
}

double add(double x, double y)
{
    return x + y;
}

double subtract(double x, double y)
{
    return x - y;
}

double multiply(double x, double y)
{
    return x * y;
}

double divide(double x, double y)
{
    return x / y;
}

double minf(double x, double y)
{
    return x <= y ? x : y;
}

double maxf(double x, double y)
{
    return x >= y ? x : y;
}