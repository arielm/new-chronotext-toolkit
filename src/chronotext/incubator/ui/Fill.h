/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Color.h"
#include "cinder/Rect.h"

namespace chr
{
    class Fill
    {
    public:
        enum
        {
            TYPE_UNDEFINED,
            TYPE_COLOR,
            TYPE_GRADIENT
        };
        
        Fill();
        Fill(const ci::ColorA &color);
        Fill(const ci::ColorA &color1, const ci::ColorA &color2, float v1 = 0, float v2 = 1);
        
        void draw(const ci::Rectf &bounds);
        
    protected:
        int type;
        ci::ColorA color;
        ci::ColorA color1;
        ci::ColorA color2;
        float v1;
        float v2;
    };
}
