#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace ci;
using namespace std;

const float GRID_SIZE = 32;
const float FRAME_SCALE = 464 / 512.0f;

void Sketch::setup(bool renew)
{
    if (renew)
    {
        textureManager.unload();
        textureManager.reload();
    }
    else
    {
        frame = textureManager.getTexture("frame rococo - 1024.png", true);
        picture = textureManager.getTexture("Louis XIV of France - 1024.jpg", true);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / (float)frame->getHeight();
    position = getWindowCenter() / scale;
}

void Sketch::draw()
{
    gl::clear(Color(0.5f, 0.5f, 0.5f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    glColor4f(1, 1, 1, 0.25f);
    glScalef(scale, scale, 1);
    drawGrid(Rectf(getWindowBounds()) / scale, GRID_SIZE, position);

    glColor4f(1, 1, 1, 1);

    glPushMatrix();
    gl::translate(position);
    glScalef(FRAME_SCALE, FRAME_SCALE, 1);
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
