#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float GRID_SIZE = 32;
const float SCALE = 768;

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.unload();
        textureManager.reload();
    }
    else
    {}
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / SCALE;
    position = getWindowCenter() / scale;
}

void Sketch::draw()
{
    gl::clear(Color::gray(0.5f), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    glColor4f(1, 1, 1, 0.25f);
    glScalef(scale, scale, 1);
    drawGrid(Rectf(getWindowBounds()) / scale, GRID_SIZE, position);
}

void Sketch::addTouch(int index, float x, float y)
{
    dragOrigin = Vec2f(x, y) / scale - position;
}

void Sketch::updateTouch(int index, float x, float y)
{
    position = Vec2f(x, y) / scale - dragOrigin;
}
