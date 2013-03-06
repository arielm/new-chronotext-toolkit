/*
 * PRODUCED BY TextureAtlas
 */

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

    Sprite(Texture *texture, float w, float h, float ox, float oy, float ow, float oh, bool rotated, float tx1, float ty1, float tx2, float ty2)
    :
    texture(texture),
    w(w),
    h(h),
    ox(ox),
    oy(oy),
    ow(ow),
    oh(oh),
    rotated(rotated),
    tx1(tx1),
    ty1(ty1),
    tx2(tx2),
    ty2(ty2)
    {}
    
    ci::Vec2f getSize() const;
    float getWidth() const;
    float getHeight() const;
    
    void beginTexture();
    void endTexture();
    
    void drawFromCenter();
    void draw(float rx = 0, float ry = 0);
};
