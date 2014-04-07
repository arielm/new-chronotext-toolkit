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

static void printRun(const TextLine &line, vector<TextRun>::const_iterator run)
{
    std::string tmp;
    line.text.tempSubString(run->start, run->end - run->start).toUTF8String(tmp);
    cout << run->tag << " [" << tmp << "]" << endl;
}

static void printRuns(const TextLine &line, vector<TextRun>::const_iterator begin, vector<TextRun>::const_iterator end)
{
    for (auto run = begin; run != end; ++run)
    {
        printRun(line, run);
    }
    
    cout << "-----" << endl;
}

StyledLineLayout::StyledLineLayout()
{}

StyledLineLayout::StyledLineLayout(const TextLine &line, map<int, ChunkStyle> &styleSheet)
{
    ZFont *currentFont = nullptr;
    auto currentBegin = line.runs.cbegin();
    
    for (auto run = line.runs.cbegin(); run != line.runs.cend(); ++run)
    {
        ZFont *font = styleSheet[run->tag].font;
        
        if (font != currentFont)
        {
            if (currentFont)
            {
                printRuns(line, currentBegin, run);
                currentBegin = run;
            }
            
            currentFont = font;
        }
    }
    
    printRuns(line, currentBegin, line.runs.cend());
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

void StyledLineLayout::setSize(float size)
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->setSize(size);
    }
}

void StyledLineLayout::beginSequence()
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->beginSequence();
    }
}

void StyledLineLayout::endSequence()
{
    for (auto &lineLayout : lineLayouts)
    {
        lineLayout->font->endSequence();
    }
}
