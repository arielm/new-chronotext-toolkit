#pragma once

#include "chronotext/path/FollowablePath.h"

class SplinePath
{
    float (*gamma)(float t, float *in);
    float tol;
    int size;
    
    std::vector<float> x;
    std::vector<float> y;

public:
    SplinePath(float (*gamma)(float t, float *in), float tol = 1, int capacity = 256);

    void clear();
    void add(float x, float y);
    void compute(FollowablePath *path);
};

float GammaCatmullRom(float t, float *in);
float GammaBSpline(float t, float *in);
