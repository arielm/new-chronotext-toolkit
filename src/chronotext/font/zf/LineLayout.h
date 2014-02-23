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
        class VirtualFont;
        
        struct Shape
        {
            hb_codepoint_t codepoint;
            ci::Vec2f position;
            
            ActualFont::Glyph *glyph;
            
            Shape(hb_codepoint_t codepoint, const ci::Vec2f &position);
        };
        
        struct Cluster
        {
            ActualFont *font;
            
            float combinedAdvance;
            std::vector<Shape> shapes;
            bool isSpace;
            
            Cluster(ActualFont *font, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
            void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
        };
        
        struct LineLayout
        {
            VirtualFont *font;
            std::string langHint;
            hb_direction_t overallDirection;
            
            std::vector<Cluster> clusters;
            
            float advance;
            float maxHeight;
            float maxAscent;
            float maxDescent;
            
            LineLayout(VirtualFont *font, const std::string &langHint, hb_direction_t overallDirection);
            void addCluster(const Cluster &cluster);
        };
    }
}

namespace chr = chronotext;
