#include "Sketch.h"

#include "chronotext/InputSource.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;
using namespace app;
using namespace chr;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (!renewContext)
    {
        dotTexture = textureManager.getTexture("dot.png", false, TextureRequest::FLAGS_TRANSLUCENT);
        
        // ---
        
        spline1 = SplinePath(InputSource::loadResource("spline_1.dat"));
        spline1.flush(SplinePath::TYPE_BSPLINE, path1, 3);
        
        // ---
        
        spline2.add(400, 100);
        spline2.add(500, 175);
        spline2.add(600, 100);
        
        spline2.add(700, 200);
        spline2.add(600, 300);
        spline2.add(500, 225);
        spline2.add(400, 300);
        
        spline2.add(300, 200);
        spline2.close();
        
        spline2.flush(SplinePath::TYPE_BSPLINE, path2, 3);
        
        // ---
        
        document = FXGDocument(InputSource::loadResource("lys.fxg"));
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    // ---
    
    gl::color(0, 0, 0, 0.5f);
    
    gl::draw(path1.getPoints());
    drawDots(spline1);
    
    // ---

    gl::color(1, 0, 0, 0.75f);
    
    gl::draw(path2.getPoints());
    drawDots(spline2);
    
    // ---
    
    gl::color(0, 0, 1, 0.5f);
    
    glPushMatrix();
    gl::translate(getWindowCenter() - document.viewSize * 0.5f);
    
    for (auto &path : document.paths)
    {
        gl::draw(path, 0.5f); // XXX: SUB-OPTIMAL (SUB-DIVIDED POINTS COULD BE CACHED)
    }
    
    glPopMatrix();
}

void Sketch::drawDots(const SplinePath &spline)
{
    dotTexture->begin();
    
    for (auto &point : spline.getPoints())
    {
        glPushMatrix();
        gl::translate(point);
        dotTexture->drawFromCenter();
        glPopMatrix();
    }
    
    dotTexture->end();
}
