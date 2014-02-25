#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float SCALE = 768;
const float TEXT_SIZE = 24;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        fontManager.discardTextures();
    }
    else
    {
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    }
    
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
    
    // ---
    
    auto matrix = font->getMatrix();
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 1);
    
    font->beginSequence();
    
    matrix->setToIdentity();
    TextHelper::drawTransformedText(*font, L"2d text ");
    
    matrix->rotateZ(9 * D2R);
    TextHelper::drawTransformedText(*font, L"is not really ");
    
    matrix->rotateZ(30 * D2R);
    TextHelper::drawTransformedText(*font, L"suited to ");
    
    matrix->rotateZ(-9 * D2R);
    TextHelper::drawTransformedText(*font, L"text trees");
    
    font->endSequence();
}
