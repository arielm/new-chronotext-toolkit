#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float SCALE = 600;
const float FPS = 30;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.unload();
        textureManager.reload();
    }
    else
    {
        animation = unique_ptr<Animation>(new Animation(make_shared<TextureAtlas>(textureManager, "MonocleMan.xml", true), InputSource::getResource("sheets.xml"), InputSource::getResource("animations.xml"), FPS));
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::resize()
{
    scale = getWindowHeight() / SCALE;
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);

    gl::translate(getWindowCenter());
    glScalef(scale, scale, 1);
    
    animation->play(getElapsedSeconds());
}
