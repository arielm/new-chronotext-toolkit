#pragma once

#include "chronotext/path/FollowablePath.h"

class SplinePath
{
    int size;
    int capacity;
    float tol;
    float (*gamma)(float t, float *in);
    float *x;
    float *y;

    void ensureCapacity(int minCapacity);

public:
    SplinePath(float (*gamma)(float t, float *in), float tol = 1, int capacity = 256);
    ~SplinePath();

    void clear();
    void add(float x, float y);
    void compute(FollowablePath *path);
};

float GammaCatmullRom(float t, float *in);
float GammaBSpline(float t, float *in);
