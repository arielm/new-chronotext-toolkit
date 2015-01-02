/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCES:
 *
 * "Spline Curves" BY Tim Lambert
 * http://www.cse.unsw.edu.au/~lambert/splines
 */

/*
 * TODO:
 *
 * IMPLEMENTING THE HERMITE CURVE (WITH BIAS AND TENSION) WOULD BE A MUST:
 * http://paulbourke.net/miscellaneous/interpolation
 */

#pragma once

#include "chronotext/path/FollowablePath.h"

namespace chr
{
    class SplinePath
    {
    public:
        enum Type
        {
            TYPE_BSPLINE,
            TYPE_CATMULL_ROM
        };
        
        SplinePath(int capacity = 0);
        SplinePath(const std::vector<ci::Vec2f> &points);
        SplinePath(InputSource::Ref inputSource);

        void read(ci::DataSourceRef source);
        void write(ci::DataTargetRef target);

        void add(const std::vector<ci::Vec2f> &points);
        void add(const ci::Vec2f &point);
        inline void add(float x, float y) { add(ci::Vec2f(x, y)); }
        
        const std::vector<ci::Vec2f>& getPoints() const;

        void clear();
        int size() const;
        bool empty() const;
        
        void close();
        bool isClosed() const;
        
        void flush(Type type, FollowablePath &path, float tol = 1) const;
        inline FollowablePath flush(Type type, float tol = 1)  const { FollowablePath path; flush(type, path, tol); return path; }
        
    protected:
        std::vector<ci::Vec2f> points;
        bool closed;
    };
    
    static ci::Vec2f GammaBSpline(float t, ci::Vec2f *in)
    {
        float w0 = ((3 - t) * t - 3) * t + 1;
        float w1 = ((3 * t - 6) * t) * t + 4;
        float w2 = ((3 - 3 * t) * t + 3) * t + 1;
        float w3 = t * t * t;
        
        return ci::Vec2f(w0 * in[0].x + w1 * in[1].x + w2 * in[2].x + w3 * in[3].x, w0 * in[0].y + w1 * in[1].y + w2 * in[2].y + w3 * in[3].y) / 6;
    }

    static ci::Vec2f GammaCatmullRom(float t, ci::Vec2f *in)
    {
        float w0 = ((2 - t) * t - 1) * t;
        float w1 = ((3 * t - 5) * t) * t + 2;
        float w2 = ((4 - 3 * t) * t + 1) * t;
        float w3 = (t - 1) * t * t;
        
        return ci::Vec2f(w0 * in[0].x + w1 * in[1].x + w2 * in[2].x + w3 * in[3].x, w0 * in[0].y + w1 * in[1].y + w2 * in[2].y + w3 * in[3].y) / 2;
    }
}
