/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

const float REFERENCE_W = 1024;
const float REFERENCE_H = 768;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.discard();
        textureManager.reload();
    }
    else
    {
        dotTexture = textureManager.getTexture("dot2x.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        
        // ---
        
        spline1 = SplinePath(InputSource::loadResource("spline_1.dat"));
        spline1.flush(SplinePath::TYPE_BSPLINE, path1, 3);
        
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
        
        spline2.flush(SplinePath::TYPE_BSPLINE, path2, 3);
        
        // ---
        
        document = FXGDocument(InputSource::loadResource("lys.fxg"));
    }

    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);
    
    // ---
    
    gl::color(0, 0, 0, 0.5f);

    glPushMatrix();
    gl::translate(-REFERENCE_W * 0.5f, -REFERENCE_H * 0.5f);

    gl::draw(path1.getPoints());
    drawDots(spline1);
    
    glPopMatrix();
    
    // ---

    gl::color(1, 0, 0, 0.75f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);
    
    gl::draw(path2.getPoints());
    drawDots(spline2);
    
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
        gl::scale(0.5f);
        dotTexture->drawFromCenter();
        glPopMatrix();
    }
    
    dotTexture->end();
}
