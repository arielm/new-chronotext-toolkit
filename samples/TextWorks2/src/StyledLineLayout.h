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
    struct Chunk
    {
        int tag;
        chr::ZFont *font;
        
        std::vector<chr::zf::Cluster*> clusters;
        
        Chunk(int tag, chr::ZFont *font)
        :
        tag(tag),
        font(font)
        {}
        
        float getAdvance() const
        {
            float advance = 0;
            
            for (auto &cluster : clusters)
            {
                advance += font->getAdvance(*cluster);
            }
            
            return advance;
        }
    };
    
    struct ChunkStyle
    {
        chr::ZFont *font;
        ci::ColorA color;
        
        ChunkStyle()
        {}
        
        ChunkStyle(std::shared_ptr<chr::ZFont> font, const ci::ColorA &color)
        :
        font(font.get()),
        color(color)
        {}
    };
    
    std::vector<std::unique_ptr<chr::zf::LineLayout>> lineLayouts;
    std::vector<Chunk> chunks;
    
    StyledLineLayout();
    StyledLineLayout(const chr::zf::TextLine &line, std::map<int, ChunkStyle> &styleSheet);
    
    float getAdvance() const;

    void setSize(float size);
    void beginSequence();
    void endSequence();
};
