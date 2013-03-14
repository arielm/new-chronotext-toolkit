#pragma once

#include "chronotext/path/FollowablePath.h"

class SplinePath
{
    std::function<float (float, float*)> gamma;
    float tol;
    int size;
    
    std::vector<float> x;
    std::vector<float> y;

public:
    SplinePath(const std::function<float (float, float*)> &gamma, float tol = 1, int capacity = 256);

    void clear();
    void add(float x, float y);
    void compute(FollowablePath *path);
};

static float GammaCatmullRom(float t, float *in)
{
    return (in[0] * ((-t + 2) * t - 1) * t + in[1] * (((3 * t - 5) * t) * t + 2) + in[2] * ((-3 * t + 4) * t + 1) * t + in[3] * ((t - 1) * t * t)) / 2;
}

static float GammaBSpline(float t, float *in)
{
    return (in[0] * (((-t + 3) * t - 3) * t + 1) + in[1] * (((3 * t - 6) * t) * t + 4) + in[2] * (((-3 * t + 3) * t + 3) * t + 1) + in[3] * (t * t * t)) / 6;
}
