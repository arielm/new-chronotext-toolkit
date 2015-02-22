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
#include "chronotext/path/FXGDocument.h"

using namespace std;
using namespace ci;
using namespace chr;

const float REFERENCE_W = 1024;
const float REFERENCE_H = 768;

void Sketch::setup()
{
    /*
     * THESE ARE MIPMAPPED TEXTURES:
     * SOME EMPTY-SPACE MUST BE LEFT AT THE EDGES IN ORDER TO AVOID ARTIFACTS WHEN SCALING-DOWN
     */
    
    roadTexture = textureManager.getTexture(Texture::Request(InputSource::getResource("asphalt_128_alpha.png"), true).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
    checkerTexture = textureManager.getTexture(Texture::Request(InputSource::getResource("checker_128.png"), true).setWrap(GL_REPEAT, GL_CLAMP_TO_EDGE));
    dotTexture = textureManager.getTexture(InputSource::getResource("dot2x.png"), true, Texture::Request::FLAGS_TRANSLUCENT);
    
    // ---
    
    SplinePath spline(InputSource::getResource("spline_1.dat"));
    
    spline.flush(SplinePath::TYPE_BSPLINE, roadPath);
    float length = roadPath.getLength();
    roadPath.setMode(FollowablePath::MODE_MODULO); // NECESSARY, FOR THE DOT TO COME-BACK TO THE START OF THE PATH ONCE THE END IS REACHED
    
    /*
     * SINGLE-PORTION TRIANGLE-STRIP
     */
    StrokeHelper::stroke(roadPath, roadStrip, 64);
    
    /*
     * MULTI-PORTION TRIANGLE-STRIP
     */
    checkerStrip.clear();
    StrokeHelper::stroke(roadPath, 0, 32, checkerStrip, 64, 2);
    StrokeHelper::stroke(roadPath, length - 32, length, checkerStrip, 64, 2, length - 32);
    
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
    peanutHairline = Hairline(textureManager, Hairline::TYPE_DASHED);
    
    // ---
    
    FXGDocument document(InputSource::getResource("lys.fxg"));
    
    for (auto &path : document.getPaths())
    {
        lys.emplace_back(make_pair(FollowablePath(path, 0.75f), Hairline(textureManager, Hairline::TYPE_NORMAL)));
    }
    
    lysOffset = -document.getViewSize() * 0.5f;
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
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
    offset = clock()->getTime() * 40;
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);
    
    // ---
    
    glPushMatrix();
    gl::translate(-REFERENCE_W * 0.5f, -REFERENCE_H * 0.5f);

    gl::color(1, 1, 1, 1);
    roadTexture->begin();
    roadStrip.draw();
    roadTexture->end();

    gl::color(1, 1, 1, 0.75f);
    checkerTexture->begin();
    checkerStrip.draw();
    checkerTexture->end();

    gl::color(1, 1, 1, 1);
    drawDotOnPath(roadPath);
    glPopMatrix();
    
    // ---
    
    gl::color(1, 0, 0, 0.5f);
    
    glPushMatrix();
    gl::translate(+REFERENCE_W * 0.25f, +REFERENCE_H * 0.25f);

    /*
     * RE-STROKING IS NECESSARY BOTH IN TERM OF POTENTIAL SCREEN-SIZE CHANGE AND IN TERM OF DASHED-LINE-OFFSET MOTION
     */
    peanutHairline.stroke(peanutPath, scale, offset);
    peanutHairline.draw();
    
    drawDotOnPath(peanutPath);
    glPopMatrix();
    
    // ---
    
    gl::color(0, 0, 1, 0.5f);
    
    glPushMatrix();
    gl::translate(lysOffset); // DRAWING THE LYS FROM ITS CENTER
    
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
    gl::translate(path.offset2Position(offset));
    gl::scale(0.5f);
    dotTexture->drawFromCenter();
    glPopMatrix();
    
    dotTexture->end();
}
