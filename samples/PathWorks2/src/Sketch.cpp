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

using namespace std;
using namespace ci;
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
        /*
         *  NECESSARY AFTER OPEN-GL CONTEXT-LOSS (OCCURS ON ANDROID WHEN APP GOES TO BACKGROUND)
         */
        textureManager.discard();
        textureManager.reload();
    }
    else
    {
        strokeTexture = textureManager.getTexture(TextureRequest(InputSource::getResource("dotted_line.png"), false, TextureRequest::FLAGS_TRANSLUCENT).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
        dotTexture = textureManager.getTexture("dot2x.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        roadTexture = textureManager.getTexture(TextureRequest(InputSource::getResource("asphalt_128_alpha.png"), true).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
        
        // ---
        
        SplinePath spline = SplinePath(InputSource::loadResource("spline_1.dat"));
        
        spline.flush(SplinePath::TYPE_BSPLINE, path1, 3);
        StrokeHelper::stroke(path1, strip1, 64);
        
        path1.setMode(FollowablePath::MODE_MODULO);
        
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

void Sketch::update()
{
    double now = getElapsedSeconds();
    position = now * 40;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);
    
    // ---
    
    gl::color(1, 1, 1, 1);
    
    glPushMatrix();
    gl::translate(-REFERENCE_W * 0.5f, -REFERENCE_H * 0.5f);
    
    roadTexture->begin();
    strip1.draw();
    roadTexture->end();
    
    drawDotOnPath(path1);
    glPopMatrix();
    
    
    // ---
    
    gl::color(0, 0, 1, 0.5f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);
    
    TexturedTriangleStrip strip;
    StrokeHelper::stroke(path2, strip, 4 / scale, 0.5f, position); // DIVIDING BY scale KEEPS THE STROKE-WIDTH "HAIRLINE"
    
    strokeTexture->begin();
    strip.draw();
    strokeTexture->end();
    
    drawDotOnPath(path2);
    glPopMatrix();
}

void Sketch::drawDotOnPath(const FollowablePath &path)
{
    dotTexture->begin();
    
    glPushMatrix();
    gl::translate(path.pos2Point(position));
    gl::scale(0.5f / scale); // DIVIDING BY SCALE KEEPS THE RADIUS CONSISTENT
    dotTexture->drawFromCenter();
    glPopMatrix();
    
    dotTexture->end();
    
}
