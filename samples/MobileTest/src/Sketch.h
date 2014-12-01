/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * IN SYNC WITH ContextRework BRANCH / COMMIT d24d1e4
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
        
        Particle()
        {}
        
        Particle(const ci::Vec2f &position, float radius, float mass = 1)
        :
        position(position),
        previousPosition(position),
        radius(radius),
        mass(mass)
        {}
    };
    
    Sketch();
    
    void setup();

    void event(int eventId);
    void start(int flags);
    void stop(int flags);

    void update();
    void draw();
    
    void drawDot(const ci::Vec2f &position, float radius, const ci::ColorA &color);
    void drawText(const std::string &text, const ci::Vec2f &position, chr::XFont::Alignment alignX, chr::XFont::Alignment alignY, float fontSize, const ci::ColorA &color);
    
    void accelerated(AccelEvent event);
    
    void accumulateForces();
    void verlet();
    void satifsfyConstraints();
    
protected:
    chr::TextureManager textureManager;
    chr::xf::FontManager fontManager;
    
    chr::TextureRef dot;
    std::shared_ptr<chr::XFont> font;
    
    float scale;

    ci::Vec2f acceleration;
    Particle particle;
};
