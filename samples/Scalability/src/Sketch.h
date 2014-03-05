/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * FEATURES:
 *
 * 1) WORKS ON OSX, iOS AND ANDROID
 * 2) DEMONSTRATES RESOLUTION-INDEPENDENT 2D SCALE
 * 3) BASIC SINGLE-FINGER DRAG
 * 4) TEXTURES RELOADED AFTER OPEN-GL CONTEXT-LOSS (NECESSARY ON ANDROID)
 * 5) TEXTURES FORCED TO POWER-OF-TWO DIMENSIONS (NECESSARY ON iOS AND ANDROID)
 *
 *
 * TODO:
 *
 * 1) ADAPT TO WINDOWS
 *    - PRE-REQUISITE:
 *      - EITHER: HANDLING RESOURCES VIA ASSETS
 *      - OR: USING THE "CINDER_RESOURCE" MACRO
 *
 *
 * REFERENCES:
 * 
 * http://commons.wikimedia.org/wiki/File:Louis_XIV_of_France.jpg
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
    void event(int id);
    void resize();
    void draw();
    
    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
};
