/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/VirtualFont.h"

class StyledLineLayout
{
public:
    struct Style
    {
        chr::ZFont *font;
        ci::ColorA color;
        
        Style()
        {}
        
        Style(std::shared_ptr<chr::ZFont> font, const ci::ColorA &color)
        :
        font(font.get()),
        color(color)
        {}
    };
    
    struct Chunk
    {
        int tag;
        chr::ZFont *font;
        ci::ColorA color;
        
        std::vector<chr::zf::Cluster*> clusters;
        
        Chunk(int tag, const Style &style)
        :
        tag(tag),
        font(style.font),
        color(style.color)
        {}
    };
    
    std::vector<std::unique_ptr<chr::zf::LineLayout>> lineLayouts;
    std::vector<Chunk> chunks;
    
    StyledLineLayout();
    StyledLineLayout(const chr::zf::TextLine &line, std::map<int, Style> &styleSheet);
    
    float getAdvance() const;
    float getOffsetX(chr::ZFont::Alignment align) const;
    float getOffsetY(chr::ZFont::Alignment align) const;
    inline ci::Vec2f getOffset(chr::ZFont::Alignment alignX, chr::ZFont::Alignment alignY) const { return ci::Vec2f(getOffsetX(alignX), getOffsetY(alignY)); }
    
    void setSize(float size) const;
    
    void beginSequence() const;
    void endSequence() const;
};
