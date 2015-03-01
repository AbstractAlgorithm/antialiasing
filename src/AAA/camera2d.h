#ifndef _core_camera2d_h_
#define _core_camera2d_h_

#include "vec2.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Camera2D
////////////////////////////////////////////////////////////////////////////////

struct Camera2D
{
    vec2 m_size;

    Camera2D(double w = 2.0, double h = 2.0)
        : m_size(w,h)
    {}

    vec2 GetTransform() { return vec2(1.0 / m_size.x, 1.0 / m_size.y); }
};

#endif