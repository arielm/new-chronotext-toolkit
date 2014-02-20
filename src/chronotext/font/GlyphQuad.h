/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "cinder/Rect.h"

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
        
        bool clip(const ci::Rectf &clipRect, const ci::Vec2f &textureFactor)
        {
            if ((x1 > clipRect.x2 ) || (x2 < clipRect.x1) || (y1 > clipRect.y2) || (y2 < clipRect.y1))
            {
                return false;
            }
            else
            {
                if (x1 < clipRect.x1)
                {
                    float dx = clipRect.x1 - x1;
                    x1 += dx;
                    u1 += dx / textureFactor.x;
                }
                
                if (x2 > clipRect.x2)
                {
                    float dx = clipRect.x2 - x2;
                    x2 += dx;
                    u2 += dx / textureFactor.x;
                }
                
                if (y1 < clipRect.y1)
                {
                    float dy = clipRect.y1 - y1;
                    y1 += dy;
                    v1 += dy / textureFactor.y;
                }
                
                if (y2 > clipRect.y2)
                {
                    float dy = clipRect.y2 - y2;
                    y2 += dy;
                    v2 += dy / textureFactor.y;
                }
                
                return true;
            }
        }
    };
}

namespace chr = chronotext;
