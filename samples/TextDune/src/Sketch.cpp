/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/MathUtils.h"
#include "chronotext/font/xf/TextHelper.h"

using namespace std;
using namespace ci;
using namespace chr;
using namespace chr::xf;

const float SCALE = 960;
const float TEXT_SIZE = 20;
const float DOT_SCALE = 0.2f;
const float GROW_FACTOR = 1.133f;

Sketch::Sketch(void *context, void *delegate)
:
CinderSketch(context, delegate)
{}

void Sketch::setup(bool renewContext)
{
    if (renewContext)
    {
        textureManager.discard();
        fontManager.discardTextures();
        
        textureManager.reload();
        fontManager.reloadTextures(); // NOT MANDATORY
    }
    else
    {
        dot = textureManager.getTexture(InputSource::getResource("dot.png"), true, TextureRequest::FLAGS_TRANSLUCENT);
        font = fontManager.getCachedFont(InputSource::getResource("Georgia_Regular_64.fnt"), XFont::Properties2d());
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::update()
{
    double now = getElapsedSeconds();

    r1 = oscillate(now, -3, +3, 0.75f);
    r2 = oscillate(now, 6, 24, 1.5f);
    r3 = oscillate(now, 15, 35, 3);
    r4 = oscillate(now, -6, +6, 1.5f);
    r5 = oscillate(now, 21, 39, 1.25f);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::translate(getWindowCenter());
    gl::scale(getWindowHeight() / SCALE);
    
    // ---

    vector<FontMatrix::Values> M;
    auto matrix = font->getMatrix();
    
    font->setSize(TEXT_SIZE);
    font->setColor(0, 0, 0, 0.75f);
    
    font->beginSequence();
    
    /*
     * THE BASE OF THE TREE IS AT THE BOTTOM OF THE SCREEN
     */
    matrix->setTranslation(0, SCALE * 0.5f);
    
    matrix->rotateZ((-90 + r1) * D2R);
    TextHelper::drawTransformedText(*font, L" 2 dimensions");
    
    matrix->rotateZ((r4) * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" are");
    
    matrix->push();
    matrix->rotateZ(+r2 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" not enough");
    
    matrix->rotateZ(r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" for");
    
    matrix->push();
    matrix->rotateZ(r2 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" text trees  ");
    M.push_back(matrix->m);

    matrix->pop();
    matrix->rotateZ(-r5 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" interaction  ");
    M.push_back(matrix->m);

    matrix->pop();
    matrix->rotateZ(-r5 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" fine");
    
    matrix->rotateZ(r4 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" for");
    
    matrix->push();
    matrix->rotateZ(-r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" text trees  ");
    M.push_back(matrix->m);
    
    matrix->pop();
    matrix->rotateZ(+r3 * D2R);
    matrix->scale(GROW_FACTOR);
    TextHelper::drawTransformedText(*font, L" fiction  ");
    M.push_back(matrix->m);
    
    font->endSequence();
    
    // ---
    
    gl::color(1, 0, 0, 0.75f);
    dot->begin();
    
    for (auto &m : M)
    {
        glPushMatrix();
        glMultMatrixf(m.data());
        
        gl::translate(0, -font->getStrikethroughOffset());
        gl::scale(DOT_SCALE);
        
        dot->drawFromCenter();
        glPopMatrix();
    }
    
    dot->end();
}

float Sketch::oscillate(double t, float min, float max, float freq)
{
    return min + 0.5 * (max - min) * (1 + math<float>::sin(t * freq));
}
