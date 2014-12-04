/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/LineLayout.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace zf
    {
        Shape::Shape(hb_codepoint_t codepoint, const Vec2f &position)
        :
        codepoint(codepoint),
        position(position),
        glyph(NULL)
        {}
        
        Cluster::Cluster(ActualFont *font, int tag, hb_codepoint_t codepoint, const Vec2f &offset, float advance)
        :
        font(font),
        tag(tag),
        combinedAdvance(advance)
        {
            shapes.emplace_back(codepoint, offset);
            
            /*
             * ASSERTION: IF THE FIRST SHAPE IS A SPACE-SEPARATOR, NO FURTHER SHAPES WILL BE ADDED
             */
            isSpace = font->isSpace(codepoint);
        }
        
        void Cluster::addShape(hb_codepoint_t codepoint, const Vec2f &offset, float advance)
        {
            shapes.emplace_back(codepoint, Vec2f(combinedAdvance, 0) + offset);
            combinedAdvance += advance;
        }
        
        LineLayout::LineLayout(VirtualFont *font, const string &langHint, hb_direction_t overallDirection)
        :
        font(font),
        langHint(langHint),
        overallDirection(overallDirection),
        advance(0),
        maxHeight(0),
        maxAscent(0),
        maxDescent(0),
        maxUnderlineOffset(0),
        maxLineThickness(0),
        averageStrikethroughOffset(0)
        {}
        
        void LineLayout::addCluster(const Cluster &cluster)
        {
            clusters.emplace_back(cluster);
            advance += cluster.combinedAdvance;
        }
    }
}
