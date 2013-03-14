/*
 * BASED ON "Adaptive Sampling of Parametric Curves"
 * BY Luiz Henrique de Figueiredo
 */

#pragma once

#include "chronotext/path/FollowablePath.h"

class ASPC
{
    float tol;
    std::function<float (float, float*)> gamma;
    FollowablePath *path;
    
    float *xx;
    float *yy;

    void sample(float t0, float x0, float y0, float t1, float x1, float y1);

public:
    ASPC(float tol, const std::function<float (float, float*)> &gamma, FollowablePath *path)
    :
    tol(tol),
    gamma(gamma),
    path(path)
    {}
    
    void segment(float *x, float *y);
};
