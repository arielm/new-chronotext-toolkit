/*
 * FEATURES:
 *
 * 1) WORKS ON OSX, iOS AND ANDROID
 * 2) DEMONSTRATES RESOLUTION-INDEPENDENT 2D SCALE
 * 3) DEMONSTRATES BASIC SINGLE-FINGER DRAG
 * 4) DEMONSTRATES AUTOMATIC TEXTURE RELOADING AFTER OPEN-GL CONTEXT-LOSS (NECESSARY ON ANDROID)
 *
 *
 * TODO:
 *
 * 1) ADAPT TO WINDOWS
 *    - PRE-REQUISITE:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: USING THE "CINDER_RESOURCE" MACRO
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    
    chr::TextureRef frame;
    chr::TextureRef picture;
    
    float scale;
    ci::Vec2f dragOrigin;
    ci::Vec2f position;

public:
    Sketch(void *context, void *delegate = NULL) : chr::CinderSketch(context, delegate) {}

    void setup(bool renewContext);
    void resize();
    void draw();
    
    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
};
