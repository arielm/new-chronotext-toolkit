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
 * OSX: COMMIT df9234d
 * IOS: COMMIT df9234d
 * ANDROID: COMMIT df9234d
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

    void event(Event event) final;
    void start(Reason reason) final;
    void stop(Reason reason) final;

    void update() final;
    void draw() final;
    
    bool keyDown(const ci::app::KeyEvent &keyEvent) final;
    
    void drawDot(const ci::Vec2f &position, float radius, const ci::ColorA &color);
    void drawText(const std::string &text, const ci::Vec2f &position, chr::XFont::Alignment alignX, chr::XFont::Alignment alignY, float fontSize, const ci::ColorA &color);
    
    void accelerated(AccelEvent event) final;
    
    void accumulateForces();
    void verlet();
    void satifsfyConstraints();
    
protected:
    chr::TextureManager textureManager;
    chr::xf::FontManager fontManager;

    chr::Texture::Ref dot;
    std::shared_ptr<chr::XFont> font;
    
    float scale;

    ci::Vec2f acceleration;
    Particle particle;
};
