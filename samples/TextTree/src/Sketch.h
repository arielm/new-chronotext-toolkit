/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) DEMONSTRATES USAGE OF chr::QuadMatrix, chr::xf::FontSequence, ETC.
 * 2) TEXT-RENDERING VIA chr::XFont
 * 3) TEXTURE LOADING AND RENDERING
 *
 * TODO:
 *
 * 1) TEXTURES SHOULD BE RENDERED VIA chr::QuadMatrix AND chr::QuadBatch
 *    - PRE-REQUISITE: TEXTURE-BATCHING SUPPORT AT THE TOOLKIT'S LEVEL
 */

/*
 * IN SYNC WITH https://github.com/arielm/new-chronotext-toolkit/tree/develop
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
    
    static float oscillate(double t, float min, float max, float freq);
};
