/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/utils/GLUtils.h"
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float TEXT_SIZE = 18;

const float REFERENCE_W = 1024;
const float REFERENCE_H = 768;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

const wstring text1 = L"followable-paths were born for motion";
const wstring text2 = L"this peanut is a B-spline with 8 points";

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.reload(); // MANDATORY
        fontManager.reloadTextures(); // NOT MANDATORY (GLYPHS TEXTURE ARE AUTOMATICALLY RELOADED WHENEVER NECESSARY)
    }
    else
    {
        dotTexture = textureManager.getTexture("dot2x.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
        
        // ---
        
        spline1 = SplinePath(InputSource::loadResource("spline_1.dat"));
        spline1.flush(SplinePath::TYPE_BSPLINE, path1);
        
        // ---
        
        spline2.add(-100, -100);
        spline2.add(   0,  -25);
        spline2.add( 100, -100);
        spline2.add( 200,    0);
        spline2.add( 100,  100);
        spline2.add(   0,   25);
        spline2.add(-100,  100);
        spline2.add(-200,    0);
        spline2.close();
        
        spline2.flush(SplinePath::TYPE_BSPLINE, path2);
        
        // ---
        
        document = FXGDocument(InputSource::loadResource("lys.fxg"));
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
            fontManager.discardTextures();
            break;
    }
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::update()
{
    double now = getElapsedSeconds();
    
    position1 = 300 + 250 * math<float>::sin(now * 1.25f);
    position2 = now * 60;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);
    
    // ---
    
    gl::color(0, 0, 0, 0.33f);

    glPushMatrix();
    gl::translate(-REFERENCE_W * 0.5f, -REFERENCE_H * 0.5f);

    gl::draw(path1.getPoints());
    drawDots(spline1);
    
    //
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    TextHelper::drawTextOnPath(*font, text1, path1, position1, -6);

    glPopMatrix();

    // ---

    gl::color(1, 0, 0, 0.5f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);
    
    gl::draw(path2.getPoints());
    drawDots(spline2);
    
    //
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    TextHelper::drawTextOnPath(*font, text2, path2, position2, font->getOffsetY(XFont::ALIGN_MIDDLE));
    
    glPopMatrix();
    
    // ---
    
    gl::color(0, 0, 1, 0.5f);
    
    glPushMatrix();
    gl::translate(-document.viewSize * 0.5f); // DRAWING THE LYS FROM ITS CENTER
    
    for (auto &path : document.paths)
    {
        gl::draw(path); // XXX: SUB-OPTIMAL (THE SUB-DIVIDED POINTS COULD BE CACHED)
    }
    
    glPopMatrix();
}

void Sketch::drawDots(const SplinePath &spline)
{
    dotTexture->begin();
    
    for (auto &point : spline.getPoints())
    {
        glPushMatrix();
        gl::translate(point);
        gl::scale(0.5f / scale); // DIVIDING BY SCALE KEEPS THE RADIUS CONSISTENT
        dotTexture->drawFromCenter();
        glPopMatrix();
    }
    
    dotTexture->end();
}
