#pragma once

#include "chronotext/cinder/CinderSketch.h"
#include "chronotext/texture/TextureManager.h"

class Sketch : public CinderSketch
{
    TextureManager textureManager;
    Texture *texture;
    
    ci::Vec2f dragOrigin;
    ci::Vec2f position;

public:
    Sketch(void *context, void *delegate = NULL)
    :
    CinderSketch(context, delegate)
    {}

    void setup(bool renew);
    void resize();
    void draw();
    
    void addTouch(int index, float x, float y);
    void updateTouch(int index, float x, float y);
};
