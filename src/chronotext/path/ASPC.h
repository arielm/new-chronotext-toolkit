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

namespace chronotext
{
    class ASPC
    {
    public:
        ASPC(FollowablePath &path, const std::function<ci::Vec2f (float, ci::Vec2f*)> &gamma, float tol);
        void segment(ci::Vec2f *point);
        
    protected:
        FollowablePath &path;
        std::function<ci::Vec2f (float, ci::Vec2f*)> gamma;
        float tol;
        
        ci::Vec2f *point;
        
        void sample(float t0, const ci::Vec2f &p0, float t1, const ci::Vec2f &p1);
    };
}

namespace chr = chronotext;
