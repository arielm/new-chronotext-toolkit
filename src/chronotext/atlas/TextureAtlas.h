/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * MANAGING SPRITES FROM TEXTURE-ATLAS MADE WITH TexturePacker
 */

#pragma once

#include "chronotext/texture/Sprite.h"
#include "chronotext/InputSource.h"

#include <map>

class TextureAtlas
{
    void init(chr::InputSourceRef inputSource, bool useMipmap);

public:
    std::map<std::string, chr::Sprite*> sprites;
    chr::TextureRef texture;

    TextureAtlas(const std::string &resourceName, bool useMipmap = false);
    TextureAtlas(chr::InputSourceRef inputSource, bool useMipmap = false);

    ~TextureAtlas();
    
    void unload();
    void reload();

    chr::Sprite* getSprite(const std::string &path);
    std::vector<chr::Sprite*> getAnimationSprites(const std::string &path) const;

    void beginTexture();
    void endTexture();

    void drawSprite(const std::string &path, float rx = 0, float ry = 0);
    void drawSpriteFromCenter(const std::string &path);
};
