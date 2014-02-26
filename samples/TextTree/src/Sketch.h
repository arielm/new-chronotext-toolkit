/*
 * ...
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
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void update();
    void draw();
    
    static float oscillate(double t, float min, float max, float freq);
};
