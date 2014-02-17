/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/ActualFont.h"

namespace chronotext
{
    namespace zf
    {
        class GlyphSequence
        {
        public:
            std::vector<ci::Vec2f> vertices;
            std::vector<ci::ColorA> colors;
            int size;
            
            GlyphSequence()
            :
            size(0)
            {}
            
            void clear()
            {
                vertices.clear();
                colors.clear();
                size = 0;
            }
            
            void addQuad(const GlyphQuad &quad, const ci::ColorA &color)
            {
                vertices.emplace_back(quad.x1, quad.y1);
                vertices.emplace_back(quad.u1, quad.v1);
                
                vertices.emplace_back(quad.x2, quad.y1);
                vertices.emplace_back(quad.u2, quad.v1);
                
                vertices.emplace_back(quad.x2, quad.y2);
                vertices.emplace_back(quad.u2, quad.v2);
                
                vertices.emplace_back(quad.x1, quad.y2);
                vertices.emplace_back(quad.u1, quad.v2);
                
                // ---
                
                colors.emplace_back(color);
                colors.emplace_back(color);
                colors.emplace_back(color);
                colors.emplace_back(color);
                
                // ---
                
                size++;
            }
        };
    }
}

namespace chr = chronotext;
