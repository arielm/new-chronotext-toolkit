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
 * OSX: COMMIT 600e7a3
 * IOS: COMMIT 600e7a3
 * ANDROID: LATEST COMMIT
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"
#include "chronotext/path/Hairline.h"

class Sketch : public chr::CinderSketch
{
public:
    void setup() final;
    void resize() final;
    void update() final;
    void draw() final;
    
    void addTouch(int index, float x, float y) final;
    void removeTouch(int index, float x, float y) final;
    
protected:
    chr::TextureManager textureManager;
    chr::xf::FontManager fontManager;
    
    std::shared_ptr<chr::XFont> font;
    
    chr::FollowablePath path;
    chr::Hairline hairline;
    std::vector<ci::Vec2f> vertices;
    
    chr::Clock::Ref slaveClock = chr::Clock::create(clock());
    
    float scale;
    float offset;
    
    void updateDune();
    void drawDune();
};
