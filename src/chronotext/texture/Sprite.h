/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/Texture.h"

namespace chr
{
    typedef std::shared_ptr<class Sprite> SpriteRef;
    
    class Sprite
    {
    public:
        TextureRef texture;
        
        float w;
        float h;
        float ox;
        float oy;
        float ow;
        float oh;
        bool rotated;
        
        float u1;
        float v1;
        float u2;
        float v2;
        
        Sprite();
        Sprite(TextureRef texture); // IMAGE-BASED
        Sprite(TextureRef texture, float w, float h, float ox, float oy, float ow, float oh, bool rotated, float u1, float v1, float u2, float v2); // ATLAS-BASED
        
        ci::Vec2f getSize() const;
        float getWidth() const;
        float getHeight() const;
        
        void beginTexture();
        void endTexture();
        
        void drawFromCenter();
        void draw(float rx = 0, float ry = 0);
    };
}
