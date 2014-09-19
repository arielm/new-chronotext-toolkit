/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

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
    chr::xf::FontManager fontManager;

    chr::TextureRef dot;
    std::shared_ptr<chr::XFont> font;

    float scale;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void event(int id);
    void draw();
    
    void drawDot(const ci::Vec2f &position, float radius, const ci::ColorA &color);
    void drawText(const std::wstring &text, const ci::Vec2f &position, chr::XFont::Alignment alignX, chr::XFont::Alignment alignY, float fontSize, const ci::ColorA &color);
};
