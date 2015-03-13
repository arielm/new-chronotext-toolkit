/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) DENSITY-BASED RENDERING:
 *    - THE PHYSICAL-SIZE OF THE GRID, THE DOT AND THE TEXT SHOULD BE IDENTICAL ON ANY (MOBILE) SCREEN
 *
 * 2) MOTION DRIVEN BY THE DEVICE'S ACCELEROMETER:
 *    - DEVICE-ORIENTATION AWARE
 *
 * 3) DEMONSTRATES HOW TO HANDLE "SKETCH LIFECYCLE" EVENTS:
 *    - GL-CONTEXT LOSS
 *    - MEMORY-WARNING
 *
 * 4) COMPLEX NATIVE INTEGRATION:
 *    - THE "SKETCH" IS NOT NECESSARILY DISPLAYED (OR EVEN "ALIVE") WHEN THE UNDERLYING "APP" STARTS...
 *    - THE UNDERLYING (OPENGL) VIEW CAN BE COMPOSITED WITH NATIVE WIDGETS
 *    - CURRENTLY DEMONSTRATED ONLY ON ANDROID:
 *      - SEE android/src/org/chronotext/MobileTest/Bridge.java
 *
 *
 * REMINDER: OSX IS USED AS A SIMULATOR, I.E.
 * - THE ACTUAL SCREEN'S DENSITY IS NOT TAKEN IN COUNT
 * - THERE IS NO ACCELEROMETER HARDWARE
 * - MEMORY IS PRACTICALLY "INFINITE"
 *
 *
 * TODO:
 *
 * 1) DEMONSTRATE "COMPLEX NATIVE INTEGRATION" ON IOS AS WELL
 */

/*
 * IN SYNC WITH develop BRANCH
 *
 * OSX: COMMIT 8358df8
 * IOS: COMMIT 8358df8
 * ANDROID: COMMIT 8358df8
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
