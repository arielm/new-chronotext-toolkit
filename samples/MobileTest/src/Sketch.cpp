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
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float REFERENCE_DENSITY = 160; // THE DENSITY-INDEPENDENT-PIXEL UNIT (DP) IS BASED ON THIS VALUE

const float DOT_RADIUS = 22; // DP
const float DOT_SCALE = 112; // DEPENDS ON IMAGE

const float FONT_SIZE = 24; // DP
const float PADDING = 20; // DP

const int FINGERS_CAPACITY = 10;
const float FINGERS_DISTANCE = 22; // DP

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.reload(); // MANDATORY
        fontManager.reloadTextures(); // NOT MANDATORY (GLYPH TEXTURES ARE AUTOMATICALLY RELOADED WHENEVER NECESSARY)
    }
    else
    {
        scale = getWindowInfo().density / REFERENCE_DENSITY;
        
        dot = textureManager.getTexture("dot_112.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Roboto_Regular_64.fnt"), XFont::Properties2d());
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
            textureManager.discard();
            break;
    }
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::color(Color::gray(0.5f));
    drawGrid(getWindowBounds(), scale * FINGERS_DISTANCE * 2, Vec2f(0, clock().getTime() * 60));

    // ---
    
    drawDot(getWindowCenter(), DOT_RADIUS, ColorA(1, 0, 0, 1));
    
    wstring text = utf8ToWstring(toString(int(clock().getTime())));
    drawText(text, Vec2f(0, getWindowHeight()) + Vec2f(PADDING, -PADDING) * scale, XFont::ALIGN_LEFT, XFont::ALIGN_BOTTOM, FONT_SIZE, ColorA(0, 0, 0, 1));
}

void Sketch::drawDot(const Vec2f &position, float radius, const ColorA &color)
{
    gl::color(color);

    glPushMatrix();
    gl::translate(position);
    gl::scale(scale * 2 * radius / DOT_SCALE);
    
    dot->begin();
    dot->drawFromCenter();
    dot->end();
    
    glPopMatrix();
}

void Sketch::drawText(const wstring &text, const Vec2f &position, XFont::Alignment alignX, XFont::Alignment alignY, float fontSize, const ColorA &color)
{
    font->setColor(color);
    font->setSize(scale * fontSize);
    
    TextHelper::drawAlignedText(*font, text, position, alignX, alignY);
}
