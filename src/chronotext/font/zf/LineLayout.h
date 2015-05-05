/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/ActualFont.h"

namespace chr
{
    namespace zf
    {
        class VirtualFont;
        
        struct Shape
        {
            hb_codepoint_t codepoint;
            ci::Vec2f position;
            
            ActualFont::Glyph *glyph = nullptr;
            
            Shape(hb_codepoint_t codepoint, const ci::Vec2f &position);
        };
        
        struct Cluster
        {
            ActualFont *font;
            int tag;
            
            float combinedAdvance;
            std::vector<Shape> shapes;
            bool isSpace;
            
            Cluster(ActualFont *font, int tag, hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
            
            void addShape(hb_codepoint_t codepoint, const ci::Vec2f &offset, float advance);
        };
        
        struct LineLayout
        {
            VirtualFont *font;
            std::string langHint;
            hb_direction_t overallDirection;
            
            std::vector<Cluster> clusters;
            
            float advance = 0;
            float maxHeight = 0;
            float maxAscent = 0;
            float maxDescent = 0;
            float maxUnderlineOffset = 0;
            float maxLineThickness = 0;
            float averageStrikethroughOffset = 0;
            
            LineLayout(VirtualFont *font, const std::string &langHint, hb_direction_t overallDirection);
            
            LineLayout(const LineLayout &other) = delete;
            void operator=(const LineLayout &other) = delete;
            
            void addCluster(Cluster &&cluster);
        };
    }
}
