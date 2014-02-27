/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
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
        ASPC(float tol, const std::function<ci::Vec2f (float, ci::Vec2f*)> &gamma, FollowablePath &path);
        
        void segment(ci::Vec2f *point);
        
    protected:
        float tol;
        std::function<ci::Vec2f (float, ci::Vec2f*)> gamma;
        FollowablePath &path;
        
        ci::Vec2f *point;
        
        void sample(float t0, float x0, float y0, float t1, float x1, float y1);
    };
}

namespace chr = chronotext;
