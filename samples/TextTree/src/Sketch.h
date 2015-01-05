/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH develop BRANCH
 *
 * OSX: COMMIT ebc399c
 * IOS: COMMIT ebc399c
 * ANDROID: COMMIT ebc399c
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void update() final;
    void draw() final;
    
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
