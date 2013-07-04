/*
 * A SIMPLE SKETCH FEATURING:
 * - RESOLUTION-INDEPENDENT 2D SCALE
 * - BASIC SINGLE-FINGER DRAG
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"

class Sketch : public CinderSketch
{
    TextureManager textureManager;
    
    TextureRef frame;
    TextureRef picture;
    
    float scale;
    ci::Vec2f dragOrigin;
    ci::Vec2f position;

public:
    Sketch(void *context, void *delegate = NULL) : CinderSketch(context, delegate) {}

    void setup(bool renew);
    void resize();
    void draw();
    
    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
};
