/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Vector.h"

namespace chronotext
{
    struct DisplayInfo
    {
        constexpr static float REFERENCE_DENSITY = 160; // THE DENSITY-INDEPENDENT-PIXEL UNIT (DP) IS BASED ON THIS VALUE
        
        ci::Vec2i size; // PIXELS
        float diagonal; // INCHES
        float density; // PIXELS/INCH
        float contentScale;
        
        DisplayInfo()
        :
        size(ci::Vec2i::zero()),
        diagonal(0),
        density(REFERENCE_DENSITY),
        contentScale(1)
        {}
    };
}

namespace chr = chronotext;
