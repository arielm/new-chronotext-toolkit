/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REFERENCES:
 *
 * 1) https://vimeo.com/14476015
 * 2) https://vimeo.com/3352337
 * 3) http://chronotext.org/bookOfSand
 */

/*
 * IN SYNC WITH develop BRANCH
 *
 * OSX: COMMIT cb97cfb
 * IOS: COMMIT cb97cfb
 * ANDROID: COMMIT cb97cfb
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"
#include "chronotext/path/Hairline.h"

class Sketch : public chr::CinderSketch
{
public:
    Sketch();
    
    void setup() final;
    void resize() final;
    void update() final;
    void draw() final;
    
    void updateDune();
    void drawDune();
    
    void addTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
    chr::TextureManager textureManager;
    chr::xf::FontManager fontManager;
    
    std::shared_ptr<chr::XFont> font;
    
    chr::FollowablePath path;
    chr::Hairline hairline;
    std::vector<ci::Vec2f> vertices;
    
    chr::Clock::Ref slaveClock;
    float scale;
    float offset;
};
