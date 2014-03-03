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
#include "chronotext/path/StrokeHelper.h"
#include "chronotext/time/Clock.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    chr::TextureRef strokeTexture;
    
    chr::xf::FontManager fontManager;
    std::shared_ptr<chr::XFont> font;
    
    chr::FollowablePath path;
    chr::TexturedTriangleStrip stroke;
    std::vector<ci::Vec2f> vertices;

    chr::Clock clock;

    float scale;
    float position;
    
public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void start(int flags);
    void stop(int flags);
    void update();
    void draw();
    
    void addTouch(int index, float x, float y);
    void removeTouch(int index, float x, float y);
    
    void createDune(const ci::Vec2f &size);
    void drawDune();
};
