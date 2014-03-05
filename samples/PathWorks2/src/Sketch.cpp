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
#include "chronotext/path/FXGDocument.h"

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
        textureManager.reload();
    }
    else
    {
        roadTexture = textureManager.getTexture(TextureRequest(InputSource::getResource("asphalt_128_alpha.png"), true).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
        dotTexture = textureManager.getTexture("dot2x.png", true, TextureRequest::FLAGS_TRANSLUCENT);
        
        // ---

        SplinePath spline = SplinePath(InputSource::loadResource("spline_1.dat"));
        
        spline.flush(SplinePath::TYPE_BSPLINE, roadPath);
        roadPath.setMode(FollowablePath::MODE_MODULO);
        
        StrokeHelper::stroke(roadPath, roadStrip, 64);
        
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
        peanutHairline = Hairline(textureManager, Hairline::TYPE_DASHED, isHighDensity());
        
        // ---
        
        FXGDocument document(InputSource::loadResource("lys.fxg"));
        
        for (auto &path : document.paths)
        {
            lys.emplace_back(make_pair(FollowablePath(path), Hairline(textureManager, Hairline::TYPE_NORMAL, isHighDensity())));
        }
        
        lysOffset = document.viewSize * 0.5f;
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

void Sketch::resize()
{
    scale = getWindowHeight() / REFERENCE_H;

    /*
     * RE-STROKING THE LYS' HAIRLINES UPON SCREEN-SIZE CHANGE:
     * NECESSARY IN ORDER TO KEEP THE STROKE-WIDTH "HAIRLINE"
     */
    for (auto &it : lys)
    {
        it.second.stroke(it.first, scale);
    }
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
    roadStrip.draw();
    roadTexture->end();
    
    drawDotOnPath(roadPath);
    glPopMatrix();
    
    
    // ---
    
    gl::color(1, 0, 0, 0.5f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);

    peanutHairline.stroke(peanutPath, scale, position); // RE-STROKING IS NECESSARY BOTH IN TERM OF SCALING AND IN TERM OF MOTION
    peanutHairline.draw();
    
    drawDotOnPath(peanutPath);
    glPopMatrix();
    
    // ---
    
    gl::color(0, 0, 1, 0.5f);
    
    glPushMatrix();
    gl::translate(-lysOffset); // DRAWING THE LYS FROM ITS CENTER
    
    for (auto &it : lys)
    {
        it.second.draw();
        drawDotOnPath(it.first);
    }
    
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
