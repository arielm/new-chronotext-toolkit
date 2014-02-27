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
const float TEXT_SIZE = 18;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.discard();
        fontManager.discardTextures();
        
        textureManager.reload();
        fontManager.reloadTextures(); // NOT MANDATORY
    }
    else
    {
        strokeTexture = textureManager.getTexture("line.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    }
    
    // ---
    
    scale = getWindowHeight() / REFERENCE_H;
    
    path = unique_ptr<FollowablePath>(new FollowablePath());
    createDune(Vec2f(getWindowSize()) / scale);

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
    gl::scale(scale);

    drawDune();
}

void Sketch::createDune(const Vec2f &size)
{
    static float coefs[] = {1.0f / 2, 1.0f / 4, 1.0f / 4 * 3, 1.0f / 2};
    int slotCount = sizeof(coefs) / sizeof(float);
    float slotSize = size.x / (slotCount - 1);

    SplinePath spline(GammaBSpline, 3);

    for (int n = 0, i = 0; i < (slotCount + 5); i++)
    {
        if (i <= 2)
        {
            n = 0; // B-SPLINE: 3 TIMES THE SAME ENTRY AT THE BEGINNING
        }
        else if (i >= slotCount + 1)
        {
            n = slotCount - 1; // B-SPLINE: 4 TIMES THE SAME ENTRY AT THE END
        }
        else
        {
            n = i - 2;
        }
        
        spline.add(slotSize * n, coefs[n] * size.y);
    }

    path->clear();
    spline.compute(*path);
    
    // ---
    
    StrokeHelper::stroke(*path, stroke, 4); // USED FOR PSEUDO-ANTIALISING
    
    // ---
    
    vertices.clear();
    vertices.reserve(path->size * 2);
    
    for (auto &point : path->points)
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
    
    strokeTexture->begin();
    stroke.draw();
    strokeTexture->end();
}
