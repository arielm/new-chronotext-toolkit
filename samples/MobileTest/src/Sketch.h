/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * ...
 */

#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "Animation.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    Animation animation;
    float scale;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void event(int id);
    void draw();
};
