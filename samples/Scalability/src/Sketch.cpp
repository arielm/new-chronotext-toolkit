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

const float PADDING1 = 0; // IN FRAME-IMAGE SPACE
const float PADDING2 = 12; // IN ABSOLUTE SCREEN SPACE

const float FRAME_INTERIOR_WIDTH = 464; // IN FRAME-IMAGE SPACE
const float GRID_SIZE = 32; // IN FRAME-IMAGE SPACE

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup()
{
    frame = textureManager.getTexture("frame rococo - 1024.png", true, TextureRequest::FLAGS_POT);
    picture = textureManager.getTexture("Louis XIV of France - 512.png", true, TextureRequest::FLAGS_POT);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    Vec2f targetSize = frame->getCleanSize() + Vec2f(PADDING1, PADDING1) * 2;
    float aspectRatio = targetSize.x / targetSize.y;
    
    if (getWindowAspectRatio() < aspectRatio)
    {
        scale = getWindowWidth() / targetSize.x;
        scale *= getWindowWidth() / (getWindowWidth() + PADDING2 * 2);
    }
    else
    {
        scale = getWindowHeight() / targetSize.y;
        scale *= getWindowHeight() / (getWindowHeight() + PADDING2 * 2);
    }
    
    // ---
    
    position = Vec2f::zero();
}

void Sketch::draw()
{
    gl::clear(Color::gray(0.5f), false);
    gl::setMatricesWindow(getWindowSize(), true);

    gl::color(1, 1, 1, 0.25f);
    drawGrid(getWindowBounds(), GRID_SIZE * scale, position * scale);

    gl::translate(getWindowCenter()); // THE ORIGIN IS AT THE CENTER OF THE SCREEN
    gl::scale(scale);

    gl::color(1, 1, 1, 1);

    glPushMatrix();
    gl::translate(position);
    gl::scale(FRAME_INTERIOR_WIDTH / picture->getCleanWidth());
    picture->begin();
    picture->drawFromCenter();
    picture->end();
    glPopMatrix();
    
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
