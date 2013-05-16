#pragma once

#include "chronotext/texture/Texture.h"

class Sprite
{
public:
    Texture *texture;

    float w;
    float h;
    float ox;
    float oy;
    float ow;
    float oh;
    bool rotated;

    float tx1;
    float ty1;
    float tx2;
    float ty2;
    
    Sprite() {}
    Sprite(Texture *texture); // IMAGE-BASED
    Sprite(Texture *texture, float w, float h, float ox, float oy, float ow, float oh, bool rotated, float tx1, float ty1, float tx2, float ty2); // ATLAS-BASED
    
    ci::Vec2f getSize() const;
    float getWidth() const;
    float getHeight() const;
    
    void beginTexture();
    void endTexture();
    
    void drawFromCenter();
    void draw(float rx = 0, float ry = 0);
};
