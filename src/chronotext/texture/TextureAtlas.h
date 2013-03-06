/*
 * MANAGING SPRITES FROM TEXTURE-ATLAS MADE WITH TexturePacker
 */

#pragma once

#include "chronotext/texture/Sprite.h"
#include "chronotext/InputSource.h"

#include <map>

class TextureAtlas
{
    std::map<std::string, Sprite*> sprites;
    
    void init(InputSourceRef inputSource, bool useMipmap);

public:
    Texture *texture;

    TextureAtlas(const std::string &resourceName, bool useMipmap = false);
    TextureAtlas(InputSourceRef inputSource, bool useMipmap = false);

    ~TextureAtlas();
    
    void unload();
    void reload();

    Sprite* getSprite(const std::string &path);
    std::vector<Sprite*> getAnimationSprites(const std::string &path) const;

    void beginTexture();
    void endTexture();

    void drawSprite(const std::string &path, float rx = 0, float ry = 0);
    void drawSpriteFromCenter(const std::string &path);
};
