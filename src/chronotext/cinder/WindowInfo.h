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
    struct WindowInfo
    {
        ci::Vec2i size;
        float contentScale;
        float diagonal; // INCHES
        float density; // PIXEL/INCH
        int aaLevel;
    };
}

namespace chr = chronotext;
