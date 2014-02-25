/*
 * ...
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/font/xf/FontManager.h"

class Sketch : public chr::CinderSketch
{
    chr::xf::FontManager fontManager;
    std::shared_ptr<chr::XFont> font;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void draw();
};
