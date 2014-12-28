/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH ResReWork BRANCH
 *
 * OSX: COMMIT 2ae1614
 * IOS: COMMIT 2ae1614
 * ANDROID: COMMIT 2ae1614
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch()
    :
    CinderSketch()
    {}
    
    void setup();
    void update();
    void draw();
    
    static float oscillate(double t, float min, float max, float freq);
    
protected:
    chr::TextureManager textureManager;
    chr::Texture::Ref dot;
    
    chr::xf::FontManager fontManager;
    std::shared_ptr<chr::XFont> font;
    
    float r1;
    float r2;
    float r3;
    float r4;
    float r5;
};
