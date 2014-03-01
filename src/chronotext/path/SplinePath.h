/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
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
        std::vector<ci::Vec2f> points;

        void add(const ci::Vec2f &point);
        void add(float x, float y);

        void clear();
        void flush(std::function<ci::Vec2f (float, ci::Vec2f*)> gamma, FollowablePath &path, float tol = 1);
    };
    
    static ci::Vec2f GammaCatmullRom(float t, ci::Vec2f *in)
    {
        float w0 = ((2 - t) * t - 1) * t;
        float w1 = ((3 * t - 5) * t) * t + 2;
        float w2 = ((4 - 3 * t) * t + 1) * t;
        float w3 = (t - 1) * t * t;
        
        return ci::Vec2f(w0 * in[0].x + w1 * in[1].x + w2 * in[2].x + w3 * in[3].x, w0 * in[0].y + w1 * in[1].y + w2 * in[2].y + w3 * in[3].y) / 2;
    }
    
    static ci::Vec2f GammaBSpline(float t, ci::Vec2f *in)
    {
        float w0 = ((3 - t) * t - 3) * t + 1;
        float w1 = ((3 * t - 6) * t) * t + 4;
        float w2 = ((3 - 3 * t) * t + 3) * t + 1;
        float w3 = t * t * t;
        
        return ci::Vec2f(w0 * in[0].x + w1 * in[1].x + w2 * in[2].x + w3 * in[3].x, w0 * in[0].y + w1 * in[1].y + w2 * in[2].y + w3 * in[3].y) / 6;
    }
}

namespace chr = chronotext;
