#pragma once

#include "chronotext/cinder/CinderSketch.h"

#include "Animation.h"

class Sketch : public chr::CinderSketch
{
    chr::TextureManager textureManager;
    std::unique_ptr<Animation> animation;
    float scale;

public:
    Sketch(void *context, void *delegate = NULL);
    
    void setup(bool renewContext);
    void resize();
    void draw();
};
