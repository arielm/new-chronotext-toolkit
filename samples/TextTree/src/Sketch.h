/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    chr::TextureRef dot;
    
    chr::xf::FontManager fontManager;
    std::shared_ptr<chr::XFont> font;
    
    float r1;
    float r2;
    float r3;
    float r4;
    float r5;
    
public:
    Sketch(void *context, void *delegate = nullptr);
    
    void setup();
    void update();
    void draw();
    
    static float oscillate(double t, float min, float max, float freq);
};
