/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/Context.h"
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

const wstring text1 = L"followable-paths were born for motion";
const wstring text2 = L"this peanut is a B-spline with 8 points";

void Sketch::setup()
{
    dotTexture = textureManager.getTexture(InputSource::getResource("dot2x.png"), true, Texture::Request::FLAGS_TRANSLUCENT);
    font = fontManager.getFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    
    // ---
    
    spline = SplinePath(InputSource::getResource("spline_1.dat"));
    spline.flush(SplinePath::TYPE_BSPLINE, path);
    
    // ---
    
    peanutSpline.add(-100, -100);
    peanutSpline.add(   0,  -25);
    peanutSpline.add( 100, -100);
    peanutSpline.add( 200,    0);
    peanutSpline.add( 100,  100);
    peanutSpline.add(   0,   25);
    peanutSpline.add(-100,  100);
    peanutSpline.add(-200,    0);
    peanutSpline.close();
    
    peanutSpline.flush(SplinePath::TYPE_BSPLINE, peanutPath);
    
    // ---
    
    FXGDocument document(InputSource::getResource("lys.fxg"));
    
    ShapeTesselator tesselator;
    tesselator.add(document);
    lys = shared_ptr<ShapeMesh>(tesselator.createMesh());
    
    lysOffset = -document.getViewSize() * 0.5f;
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    if (getWindowInfo().aaLevel < 4)
    {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::update()
{
    double now = clock()->getTime();
    
    offset1 = 300 + 250 * math<float>::sin(now * 1.25f);
    offset2 = now * 60;
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

    gl::draw(path.getPoints());
    drawDots(spline);
    
    //
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    TextHelper::drawTextOnPath(*font, text1, path, offset1, -6);

    glPopMatrix();

    // ---

    gl::color(1, 0, 0, 0.5f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);
    
    gl::draw(peanutPath.getPoints());
    drawDots(peanutSpline);
    
    //
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    TextHelper::drawTextOnPath(*font, text2, peanutPath, offset2, font->getOffsetY(XFont::ALIGN_MIDDLE));
    
    glPopMatrix();
    
    // ---
    
    gl::color(0, 0.33f, 1, 0.67f);
    
    glPushMatrix();
    gl::translate(lysOffset); // DRAWING THE LYS FROM ITS CENTER
    lys->draw();
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
