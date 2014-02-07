/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/path/FollowablePath.h"

namespace chronotext
{
    class SplinePath
    {
    public:
        SplinePath(const std::function<float (float, float*)> &gamma, float tol = 1, int capacity = 256);
        
        inline void add(const ci::Vec2f &point) { add(point.x, point.y); }
        void add(float x, float y);

        void clear();
        void compute(FollowablePath *path);
        
    protected:
        std::function<float (float, float*)> gamma;
        float tol;
        int size;
        
        std::vector<float> x;
        std::vector<float> y;
    };
    
    static float GammaCatmullRom(float t, float *in)
    {
        return (in[0] * ((-t + 2) * t - 1) * t + in[1] * (((3 * t - 5) * t) * t + 2) + in[2] * ((-3 * t + 4) * t + 1) * t + in[3] * ((t - 1) * t * t)) / 2;
    }
    
    static float GammaBSpline(float t, float *in)
    {
        return (in[0] * (((-t + 3) * t - 3) * t + 1) + in[1] * (((3 * t - 6) * t) * t + 4) + in[2] * (((-3 * t + 3) * t + 3) * t + 1) + in[3] * (t * t * t)) / 6;
    }
}

namespace chr = chronotext;
