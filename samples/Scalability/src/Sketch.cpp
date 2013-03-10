#include "Sketch.h"
#include "Resources.h"

#include "chronotext/utils/glUtils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const float GRID_SIZE = 32;
const float FRAME_SCALE = 464 / 512.0;

void Sketch::setup(bool renew)
{
    if (renew)
    {
        textureManager.unload(); // KIND OF NO-OP IN THE CASE OF TextureManager, BUT NECESSARY TO BE CALLED BEFORE reload()
        textureManager.reload();
    }
    else
    {
        frame = textureManager.getTexture(RES_FRAME, true);
        picture = textureManager.getTexture(RES_PICTURE, true);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / (float) frame->getHeight();
    position = getWindowCenter() / scale;
    
    gl::setMatricesWindow(getWindowSize(), true);
}

void Sketch::draw()
{
    gl::clear(Color(0.5f, 0.5f, 0.5f), false);

    glLoadIdentity();
    glScalef(scale, scale, 1);
    
    glColor4f(1, 1, 1, 0.25f);
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
