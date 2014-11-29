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

void Sketch::setup()
{
    auto windowInfo = getWindowInfo();
    float scale = windowInfo.density / 160;
    
    fontManager.loadConfig(InputSource::getResource("font-config.xml"));
    font = fontManager.getCachedFont("serif", ZFont::STYLE_REGULAR, ZFont::Properties2d(scale * FONT_SIZE).setCrisp());
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);

    // ---
    
    font->setColor(0, 0, 0, 0.85f);
    drawAlignedText(*font, TEXT, getWindowCenter(), ZFont::ALIGN_MIDDLE, ZFont::ALIGN_MIDDLE);
}

void Sketch::drawAlignedText(ZFont &font, const string &text, const Vec2f &position, ZFont::Alignment alignX, ZFont::Alignment alignY)
{
    auto layout = font.getCachedLineLayout(text);
    Vec2f p = position + font.getOffset(*layout, alignX, alignY);
    
    font.beginSequence();
    
    for (auto &cluster : layout->clusters)
    {
        font.addCluster(cluster, p);
        p.x += font.getAdvance(cluster);
    }
    
    font.endSequence();
}
