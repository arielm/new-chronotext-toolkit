/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * BASED ON "Adaptive Sampling of Parametric Curves" BY Luiz Henrique de Figueiredo
 * http://ariel.chronotext.org/dd/defigueiredo93adaptive.pdf
 */

#pragma once

#include "chronotext/path/FollowablePath.h"

class ASPC
{
public:
    ASPC(std::function<ci::Vec2f (float, ci::Vec2f*)> gamma, chr::FollowablePath &path, float tol)
    :
    gamma(gamma),
    path(path),
    tol(tol)
    {}
    
    std::function<ci::Vec2f (float, ci::Vec2f*)> gamma;
    chr::FollowablePath &path;
    float tol;
    
    void segment(ci::Vec2f *point)
    {
        this->point = point;
        
        float pt = 0;
        auto p = gamma(pt, point);
        
        float qt = 1;
        auto q = gamma(qt, point);
        
        sample(pt, p, qt, q);
    }
    
protected:
    ci::Vec2f *point;
    
    void sample(float t0, const ci::Vec2f &p0, float t1, const ci::Vec2f &p1)
    {
        float t = 0.45f + 0.1f * rand() / RAND_MAX;
        float rt = t0 + t * (t1 - t0);
        auto r = gamma(rt, point);
        
        float cross = (p0 - r).cross(p1 - r);
        
        if (cross * cross < tol)
        {
            path.add(p0);
        }
        else
        {
            sample(t0, p0, rt, r);
            sample(rt, r, t1, p1);
        }
    }
};
