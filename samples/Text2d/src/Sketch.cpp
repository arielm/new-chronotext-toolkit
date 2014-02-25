#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float SCALE = 600;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {}
    else
    {}
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::clear(Color::gray(1.0f), false);
    gl::setMatricesWindow(getWindowSize(), true);

    gl::translate(getWindowCenter());
    gl::scale(getWindowHeight() / SCALE);
}
