#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float GRID_SIZE = 32;
const float FRAME_SCALE = 464 / 512.0f;

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        /*
         *  NECESSARY AFTER OPEN-GL CONTEXT-LOSS (OCCURS ON ANDROID WHEN APP GOES TO BACKGROUND)
         */
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
    
    gl::color(1, 1, 1, 0.25f);
    gl::scale(scale);
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
