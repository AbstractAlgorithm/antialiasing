#ifndef _core_timer_h_
#define _core_timer_h_

#include <windows.h>
#include <stdint.h>

namespace Timer
{
    static LARGE_INTEGER qpFrequency;
    static LARGE_INTEGER prevFrameTime, nowTime;
    double DIFF = 1.0;
    double FPS = 0.0;

    void Init()
    {
        QueryPerformanceFrequency(&qpFrequency);
        QueryPerformanceCounter(&prevFrameTime);
    }

    void Update()
    {
        QueryPerformanceCounter(&nowTime);
        LARGE_INTEGER elapsed;
        elapsed.QuadPart = nowTime.QuadPart - prevFrameTime.QuadPart;
        elapsed.QuadPart *= 1000000;
        elapsed.QuadPart /= qpFrequency.QuadPart;
        DIFF = (double)elapsed.QuadPart;
        FPS = 1000000.0 / DIFF;
        prevFrameTime = nowTime;
    }
}

#endif