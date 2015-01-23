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
 * OSX: COMMIT 49c2b15
 * IOS: COMMIT 49c2b15
 * ANDROID: COMMIT 49c2b15
 */

/*
 * DESKTOP ONLY: PRESS W TO SIMULATE "MEMORY WARNING"
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/font/xf/FontManager.h"

class Sketch : public chr::CinderSketch
{
public:
    struct Particle
    {
        ci::Vec2f position;
        ci::Vec2f previousPosition;
        ci::Vec2f acceleration;
        
        float radius;
        float mass;
        
        Particle() = default;
        
        Particle(const ci::Vec2f &position, float radius, float mass = 1)
        :
        position(position),
        previousPosition(position),
        radius(radius),
        mass(mass)
        {}
    };
    
    // ---
    
    void setup() final;

    void start(StartReason reason) final;
    void stop(StopReason reason) final;

    void update() final;
    void draw() final;

    void event(int eventId) final;

    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    
    void accelerated(AccelEvent event) final;
    
protected:
    chr::TextureManager textureManager;
    chr::xf::FontManager fontManager;

    chr::Texture::Ref dot;
    std::shared_ptr<chr::XFont> font;
    
    float scale;

    ci::Vec2f acceleration;
    Particle particle;
    
    void drawDot(const ci::Vec2f &position, float radius, const ci::ColorA &color);
    void drawText(const std::string &text, const ci::Vec2f &position, chr::XFont::Alignment alignX, chr::XFont::Alignment alignY, float fontSize, const ci::ColorA &color);
    
    void accumulateForces();
    void verlet();
    void satifsfyConstraints();
};
