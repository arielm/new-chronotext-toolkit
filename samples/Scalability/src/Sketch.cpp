#include "Sketch.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void Sketch::setup(bool renew)
{
    if (renew)
    {
        textureManager.unload(); // KIND OF NO-OP IN THE CASE OF TextureManager, BUT NECESSARY TO BE CALLED BEFORE reload()
        textureManager.reload();
    }
    else
    {
        texture = textureManager.getTexture(RES_CHINA);
        position = getWindowSize() * 0.5f;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    gl::setMatricesWindow(getWindowSize(), true);
}

void Sketch::draw()
{
    gl::clear(Color(0.5f, 0.5f, 0.5f), false);
    glLoadIdentity();
    
    // ---

    gl::color(ColorA::white());
    gl::translate(position);

    texture->begin();
    texture->drawFromCenter();
    texture->end();
}

void Sketch::addTouch(int index, float x, float y)
{
    dragOrigin = Vec2f(x, y) - position;
}

void Sketch::updateTouch(int index, float x, float y)
{
    position = Vec2f(x, y) - dragOrigin;
}
