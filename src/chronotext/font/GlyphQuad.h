/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

namespace chronotext
{
    struct GlyphQuad
    {
        float x1;
        float y1;
        float x2;
        float y2;
        
        float u1;
        float v1;
        float u2;
        float v2;
    };
}

namespace chr = chronotext;
