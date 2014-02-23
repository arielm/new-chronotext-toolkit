/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/LineLayout.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    namespace zf
    {
        Shape::Shape(hb_codepoint_t codepoint, const Vec2f &position)
        :
        codepoint(codepoint),
        position(position)
        {}
        
        Cluster::Cluster(ActualFont *font, hb_codepoint_t codepoint, const Vec2f &offset, float advance)
        :
        font(font),
        combinedAdvance(advance)
        {
            shapes.emplace_back(codepoint, offset);
            
            /*
             * ASSERTION: IF THE FIRST SHAPE IS A SPACE-SEPARATOR, NO SHAPES WILL FOLLOW
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
        maxDescent(0)
        {}
        
        void LineLayout::addCluster(const Cluster &cluster)
        {
            clusters.emplace_back(cluster);
            advance += cluster.combinedAdvance;
        }
    }
}
