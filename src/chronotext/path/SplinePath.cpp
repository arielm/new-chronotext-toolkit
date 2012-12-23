#include "chronotext/path/SplinePath.h"
#include "chronotext/path/ASPC.h"

#define CAPACITY_INCREMENT 0 /* 0 MEANS THAT CAPACITY IS MULTIPLIED BY 2 WHEN NECESSARY */

SplinePath::SplinePath(float (*gamma)(float t, float *in), float tol, int capacity)
:
gamma(gamma),
tol(tol),
capacity(capacity),
size(0)
{
    x = new float[capacity];
    y = new float[capacity];
}

SplinePath::~SplinePath()
{
    delete[] x;
    delete[] y;
}

void SplinePath::ensureCapacity(int minCapacity)
{
    if (minCapacity > capacity)
    {
        capacity = (CAPACITY_INCREMENT > 0) ? (capacity + CAPACITY_INCREMENT) : (capacity * 2);
        
        x = (float*) realloc(x, capacity * sizeof(float));
        y = (float*) realloc(y, capacity * sizeof(float));
    }
}

void SplinePath::clear()
{
    size = 0;
}

void SplinePath::add(float xx, float yy)
{
    ensureCapacity(size + 1);
    
    x[size] = xx;
    y[size] = yy;
    size++;
}

void SplinePath::compute(FollowablePath *path)
{
    ASPC aspc(tol, gamma, path);
    
    for (int i = 0; i < size - 3; i++)
    {
        aspc.segment(x + i, y + i);
    }
    
    if (path->mode == FollowablePath::MODE_LOOP)
    {
        path->add(path->x[0], path->y[0]);
    }
}

float GammaCatmullRom(float t, float *in)
{
    return (in[0] * ((-t + 2) * t - 1) * t + in[1] * (((3 * t - 5) * t) * t + 2) + in[2] * ((-3 * t + 4) * t + 1) * t + in[3] * ((t - 1) * t * t)) / 2;
}

float GammaBSpline(float t, float *in)
{
    return (in[0] * (((-t + 3) * t - 3) * t + 1) + in[1] * (((3 * t - 6) * t) * t + 4) + in[2] * (((-3 * t + 3) * t + 3) * t + 1) + in[3] * (t * t * t)) / 6;
}
