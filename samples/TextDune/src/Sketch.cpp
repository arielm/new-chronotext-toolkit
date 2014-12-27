/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"
#include "chronotext/path/SplinePath.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float REFERENCE_H = 768;
const float TEXT_SIZE = 20;
const float GAP = 7;

const wstring text = L"hell with sinus, text should move under the influence of physical forces!";

Sketch::Sketch()
:
CinderSketch(),
slaveClock(Clock::create(clock()))
{}

void Sketch::setup()
{
    hairline = Hairline(textureManager, Hairline::TYPE_NORMAL);
    font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
    updateDune();
}

void Sketch::update()
{
    offset = 600 + 325 * math<float>::sin(slaveClock.getTime() * 1.75f);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    
    gl::setMatricesWindow(getWindowSize(), true);
    gl::scale(scale);

    // ---
    
    drawDune();
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.85f);
    TextHelper::drawTextOnPath(*font, text, path, offset, -GAP);
}

void Sketch::updateDune()
{
    const float coefs[] = {1.0f / 2, 1.0f / 4, 1.0f / 4 * 3, 1.0f / 2};
    const int slotCount = sizeof(coefs) / sizeof(float);

    Vec2f size = Vec2f(getWindowSize()) / scale;
    float slotSize = size.x / (slotCount - 1);
    
    SplinePath spline;

    for (int i = 0; i < slotCount; i++)
    {
        spline.add(slotSize * i, coefs[i] * size.y);
    }

    path.clear();
    spline.flush(SplinePath::TYPE_BSPLINE, path, 3); // USING A TOLERANCE OF 3: REDUCING SEGMENTS WHILE PRESERVING SMOOTHNESS
    
    // ---
    
    hairline.stroke(path, scale); // USED FOR PSEUDO-ANTIALISING
    
    // ---
    
    vertices.clear();
    vertices.reserve(path.size() * 2);
    
    for (auto &point : path.getPoints())
    {
        vertices.emplace_back(point);
        vertices.emplace_back(point.x, REFERENCE_H);
    }
}

void Sketch::drawDune()
{
    gl::color(1, 0, 0, 0.5f);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertices.data());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    
    // ---
    
    gl::color(1, 1, 1, 1);
    hairline.draw();
}

void Sketch::addTouch(int index, float x, float y)
{
    slaveClock.stop();
}

void Sketch::removeTouch(int index, float x, float y)
{
    slaveClock.start();
}
