/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/Utils.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::zf;

const float FONT_SIZE = 64; // THE MAXIMAL FONT-SIZE

void Sketch::setup()
{
    fontManager.loadConfig(InputSource::getResource("font-config.xml"));
    
    font1 = fontManager.getCachedFont("serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(FONT_SIZE));
    font2 = fontManager.getCachedFont("serif", ZFont::STYLE_BOLD, ZFont::Properties2d(FONT_SIZE));
    
    styleSheet[0] = StyledLineLayout::Style(font1, ColorA(0, 0, 0, 0.85f));
    styleSheet[1] = StyledLineLayout::Style(font2, ColorA(0, 0, 0, 0.85f));
    styleSheet[2] = StyledLineLayout::Style(font1, ColorA(1, 0, 0, 0.75f));
    
    // ---
    
    TextLine line;
    line.addChunk("Spouse and ", 0);
    line.addChunk("helpmate", 1);
    line.addChunk(" of אָדָם קַדְמוֹן: Heva, ", 0);
    line.addChunk("naked", 2);
    line.addChunk(" Eve", 0);
    
    fontManager.itemizer->processLine(line);
    layout = StyledLineLayout(line, styleSheet);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    /*
     * COMPUTING THE FONT-SIZE SO THAT:
     * - TEXT IS AT MOST AS WIDE AS 85% OF THE SCREEN
     * - THE MAXIMAL FONT-SIZE IS NOT EXCEEDED
     */
    
    layout.setSize(FONT_SIZE);
    float width = layout.getAdvance();
    float availableWidth = getWindowWidth() * 0.85f;
    
    fontSize = std::min(FONT_SIZE, FONT_SIZE * availableWidth / width);
    layout.setSize(fontSize);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);

    // ---
    
    drawAlignedText(layout, getWindowCenter(), ZFont::ALIGN_MIDDLE, ZFont::ALIGN_MIDDLE);
}

void Sketch::drawAlignedText(const StyledLineLayout &layout, const Vec2f &position, ZFont::Alignment alignX, ZFont::Alignment alignY)
{
    Vec2f p = position + layout.getOffset(alignX, alignY);
    
    layout.beginSequence();
    
    for (auto &chunk : layout.chunks)
    {
        chunk.font->setColor(chunk.color);
        
        for (auto &cluster : chunk.range)
        {
            chunk.font->addCluster(cluster, p);
            p.x += chunk.font->getAdvance(cluster);
        }
    }
    
    layout.endSequence();
}
