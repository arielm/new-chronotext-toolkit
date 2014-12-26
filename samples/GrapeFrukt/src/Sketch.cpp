/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "Sketch.h"

#include "chronotext/utils/GLUtils.h"

using namespace std;
using namespace ci;
using namespace chr;

const float REFERENCE_H = 600;
const float FPS = 30;

void Sketch::setup()
{
    auto atlas = make_shared<TextureAtlas>(textureManager, InputSource::getResource("MonocleMan.xml"), true);
    animation = Animation(atlas, InputSource::getResource("sheets.xml"), InputSource::getResource("animations.xml"), FPS);
    
    // ---
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}

void Sketch::draw()
{
    gl::clear(Color::white(), false);
    gl::setMatricesWindow(getWindowSize(), true);
    
    gl::color(Color::gray(0.5f));
    utils::gl::drawGrid(getWindowBounds(), 64, Vec2f(0, clock().getTime() * 60));

    gl::translate(getWindowCenter());
    gl::scale(getWindowHeight() / REFERENCE_H);
    
    animation.play(clock().getTime());
}
