/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * ADD SUPPORT FOR BEZIER CURVES:
 * https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/Path2d.cpp#L653-657
 * https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/Path2d.cpp#L689-697
 */

#pragma once

#include "chronotext/path/FollowablePath.h"

namespace chronotext
{
    class SplinePath
    {
    public:
        SplinePath(const std::function<float (float, float*)> &gamma, float tol = 1, int capacity = 256);
        
        void add(float x, float y);
        inline void add(const ci::Vec2f &point) { add(point.x, point.y); }

        void clear();
        void compute(FollowablePath &path);
        
    protected:
        std::function<float (float, float*)> gamma;
        float tol;
        
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
