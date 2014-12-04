/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * MANAGING SPRITES FROM TEXTURE-ATLAS MADE WITH TexturePacker:
 * http://www.codeandweb.com/texturepacker
 *
 * THE UNDERLYING TEXTURE IS CONTROLLED BY TextureManager, I.E.
 * IT CAN BE AUTOMATICALLY RELOADED AFTER OPEN-GL CONTEXT-LOSS
 */

#pragma once

#include "chronotext/texture/TextureManager.h"
#include "chronotext/texture/Sprite.h"
#include "chronotext/InputSource.h"

#include <map>

namespace chr
{
    class TextureAtlas
    {
    public:
        TextureRef texture;
        
        TextureAtlas(TextureManager &textureManager, const std::string &resourceName, bool useMipmap = false);
        TextureAtlas(TextureManager &textureManager, InputSourceRef inputSource, bool useMipmap = false);
        
        SpriteRef getSprite(const std::string &path) const;
        std::vector<SpriteRef> getAnimationSprites(const std::string &path) const;
        
        void beginTexture();
        void endTexture();
        
        void drawSprite(const std::string &path, float rx = 0, float ry = 0);
        void drawSpriteFromCenter(const std::string &path);
        
    protected:
        TextureManager &textureManager;
        std::map<std::string, SpriteRef> sprites;
        
        void init(InputSourceRef inputSource, bool useMipmap);
    };
}
