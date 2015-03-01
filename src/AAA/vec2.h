#ifndef _core_vec2_h_
#define _core_vec2_h_

#include <math.h>

struct vec2
{
    double x, y;

    vec2(double _x = 0.0, double _y = 0.0)
        : x(_x)
        , y(_y)
    {}

    double len()
    {
        return sqrt(x*x + y*y);
    }
    vec2& normalize()
    {
        double l = len();
        if (l > 0.00001)
            *this /= l;
        return *this;
    }
    vec2& operator=(vec2& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }
    vec2 operator-(vec2& v)
    {
        return vec2(x - v.x, y - v.y);
    }
    vec2& operator-=(vec2& v)
    {
        return *this = *this - v;
    }
    vec2 operator+(vec2& v)
    {
        return vec2(x + v.x, y + v.y);
    }
    vec2& operator+=(vec2& v)
    {
        return *this = *this + v;
    }
    vec2 operator/(double s)
    {
        return vec2(x / s, y / s);
    }
    vec2& operator/=(double s)
    {
        return *this = *this / s;
    }
    vec2 operator*(double s)
    {
        return vec2(x * s, y * s);
    }
    vec2& operator*=(double s)
    {
        return *this = *this * s;
    }
    double operator*(vec2& v)
    {
        return x*v.y - y*v.x;
    }
#ifdef DEBUGMODE
    void write()
    {
        printf("(%.2lf, %.2lf)", x, y);
    }
#endif
};

#endif