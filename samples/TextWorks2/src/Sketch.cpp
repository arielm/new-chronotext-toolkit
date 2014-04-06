/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/Utils.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace zf;

const float FONT_SIZE = 24; // SIZE IN PIXELS (CORRESPONDS TO 0.15 INCHES AT 160 DPI)
const string TEXT = "Spouse and helpmate of אָדָם קַדְמוֹן: Heva, naked Eve"; // FROM JAMES JOYCE'S ULYSSES, WITH "ADAM KADMON" IN HEBREW

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (!renewContext)
    {
        auto windowInfo = getWindowInfo();
        float scale = windowInfo.density / 160;
        
        fontManager.loadConfig(InputSource::getResource("font-config.xml"));
        font = fontManager.getCachedFont("serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(scale * FONT_SIZE).setCrisp());

        // ---
        
        TextLine line;
        line.addChunk("Spouse and ", 0);
        line.addChunk("helpmate", 1);
        line.addChunk(" of אָדָם קַדְמוֹן: Heva, ", 0);
        line.addChunk("naked", 2);
        line.addChunk(" Eve", 0);
        
        fontManager.itemizer.processLine(line);
        
        for (auto &run : line.runs)
        {
            std::string tmp;
            line.text.tempSubString(run.start, run.end - run.start).toUTF8String(tmp);
            
            cout << run.tag << " [" << tmp << "]" << endl;
        }
        
        layout = unique_ptr<LineLayout>(font->createLineLayout(TEXT));
    }
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::event(int id)
{
    switch (id)
    {
        case EVENT_CONTEXT_LOST:
            fontManager.discardTextures();
            break;
    }
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);

    // ---
    
    font->setColor(0, 0, 0, 0.85f);
    drawAlignedText(*font, *layout, getWindowCenter(), ZFont::ALIGN_MIDDLE, ZFont::ALIGN_MIDDLE);
}

void Sketch::drawAlignedText(ZFont &font, const LineLayout &layout, const Vec2f &position, ZFont::Alignment alignX, ZFont::Alignment alignY)
{
    Vec2f p = position + font.getOffset(layout, alignX, alignY);
    
    font.beginSequence();
    
    for (auto &cluster : layout.clusters)
    {
        font.addCluster(cluster, p);
        p.x += font.getAdvance(cluster);
    }
    
    font.endSequence();
}
