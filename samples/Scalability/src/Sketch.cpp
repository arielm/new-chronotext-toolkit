/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float FRAME_SCALE = 464 / 512.0f; // RATIO BETWEEN THE FRAME'S INTERIOR AND THE PICTURE'S WIDTH
const float GRID_SIZE = 32;

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
        frame = textureManager.getTexture("frame rococo - 1024.png", true, TextureRequest::FLAGS_POT); // FORCING SIZE FROM 656x1024 TO 1024x1024 (NECESSARY ON iOS AND ANDROID)
        picture = textureManager.getTexture("Louis XIV of France - 1024.jpg", true); // ALREADY POWER-OF-TWO (512x1024)
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    Vec2f targetSize = frame->getCleanSize();
    float aspectRatio = targetSize.x / targetSize.y;
    
    if (getWindowAspectRatio() < aspectRatio)
    {
        scale = getWindowWidth() / targetSize.x;
    }
    else
    {
        scale = getWindowHeight() / targetSize.y;
    }
    
    // ---
    
    position = getWindowCenter() / scale;
}

void Sketch::draw()
{
    gl::clear(Color::gray(0.5f), false);
    
    gl::setMatricesWindow(getWindowSize(), true);
    gl::scale(scale);
    
    gl::color(1, 1, 1, 0.25f);
    drawGrid(Rectf(getWindowBounds()) / scale, GRID_SIZE, position);

    gl::color(1, 1, 1, 1);

    glPushMatrix();
    gl::translate(position);
    gl::scale(FRAME_SCALE);
    picture->begin();
    picture->drawFromCenter();
    picture->end();
    glPopMatrix();
    
    gl::translate(getWindowCenter() / scale);
    frame->begin();
    frame->drawFromCenter();
    frame->end();
}

void Sketch::addTouch(int index, float x, float y)
{
    dragOrigin = Vec2f(x, y) / scale - position;
}

void Sketch::updateTouch(int index, float x, float y)
{
    position = Vec2f(x, y) / scale - dragOrigin;
}
