/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "StyledLineLayout.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace zf;

StyledLineLayout::StyledLineLayout()
{}

StyledLineLayout::StyledLineLayout(const TextLine &line, map<int, Style> &styleSheet)
{
    ZFont *currentFont = nullptr;
    auto currentBegin = line.runs.cbegin();
    
    for (auto run = line.runs.cbegin(); run != line.runs.cend(); ++run)
    {
        auto font = styleSheet[run->tag].font;
        
        if (font != currentFont)
        {
            if (currentFont)
            {
                lineLayouts.emplace_back(unique_ptr<LineLayout>(currentFont->createLineLayout(line, currentBegin, run)));
                currentBegin = run;
            }
            
            currentFont = font;
        }
    }

    if (currentFont)
    {
        lineLayouts.emplace_back(unique_ptr<LineLayout>(currentFont->createLineLayout(line, currentBegin, line.runs.cend())));
    }
    
    // ---
    
    for (auto &lineLayout : lineLayouts)
    {
        int currentTag = -1;
        
        for (auto &cluster : lineLayout->clusters)
        {
            if (cluster.tag != currentTag)
            {
                currentTag = cluster.tag;
                chunks.emplace_back(currentTag, styleSheet[currentTag]);
            }
            
            chunks.back().clusters.push_back(&cluster);
        }
    }
}

float StyledLineLayout::getAdvance() const
{
    float advance = 0;
    
    for (auto &lineLayout : lineLayouts)
    {
        advance += lineLayout->font->getAdvance(*lineLayout);
    }
    
    return advance;
}

float StyledLineLayout::getOffsetX(ZFont::Alignment align) const
{
    switch (align)
    {
        case ZFont::ALIGN_MIDDLE:
            return -0.5f * getAdvance();
            
        case ZFont::ALIGN_RIGHT:
            return -getAdvance();
            
        default:
            return 0;
    }
}

float StyledLineLayout::getOffsetY(ZFont::Alignment align) const
{
    float maxAscent = 0;
    float maxDescent = 0;
    
    for (auto &lineLayout : lineLayouts)
    {
        maxAscent = std::max(maxAscent, lineLayout->font->getAscent(*lineLayout));
        maxDescent = std::max(maxDescent, lineLayout->font->getDescent(*lineLayout));
    }
    
    // ---
    
    switch (align)
    {
        case ZFont::ALIGN_MIDDLE:
            return 0.5f * (maxAscent - maxDescent);
            
        case ZFont::ALIGN_TOP:
            return +maxAscent;
            
        case ZFont::ALIGN_BOTTOM:
            return -maxDescent;
            
        default:
            return 0;
    }
}

void StyledLineLayout::setSize(float size) const
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->setSize(size);
    }
}

void StyledLineLayout::beginSequence() const
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->beginSequence(true);
    }
}

void StyledLineLayout::endSequence() const
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->endSequence();
    }
}
