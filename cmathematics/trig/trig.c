#include "trig.h"

#define _USE_MATH_DEFINES
#include <math.h>

float degToRad(float deg)
{
    return deg * M_PI / 180.0f;
}

float radToDeg(float rad)
{
    return rad * 180.0f / M_PI;
}

float invtan(float x, float y)
{
    if (x == 0.0f)
    {
        if (y > 0.0f)
        {
            return M_PI_2;
        }
        if (y < 0.0f)
        {
            return -M_PI_2;
        }
        
        return NAN;
    }

    if (x > 0.0f)
    {
        return atanf(y / x);
    }
    
    if (x < 0.0f)
    {
        if (y >= 0.0f)
        {
            return atanf(y / x) + M_PI;
        }

        return atanf(y / x) - M_PI;
    }
}