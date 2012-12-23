#include "chronotext/path/ASPC.h"

#include <stdlib.h>

void ASPC::sample(float t0, float x0, float y0, float t1, float x1, float y1)
{
    float t = 0.45f + 0.1f * (float) (rand() / RAND_MAX);
    float rt = t0 + t * (t1 - t0);
    float rx = gamma(rt, in_x);
    float ry = gamma(rt, in_y);
    
    float cross = (x0 - rx) * (y1 - ry) - (x1 - rx) * (y0 - ry);
    if ((cross * cross) < tol)
    {
        path->add(x0, y0);
    }
    else
    {
        sample(t0, x0, y0, rt, rx, ry);
        sample(rt, rx, ry, t1, x1, y1);
    }
}

void ASPC::segment(float *x, float *y)
{
    in_x = x;
    in_y = y;
    
    float pt = 0;
    float px = gamma(pt, in_x);
    float py = gamma(pt, in_y);
    
    float qt = 1;
    float qx = gamma(qt, in_x);
    float qy = gamma(qt, in_y);
    
    sample(pt, px, py, qt, qx, qy);
}
