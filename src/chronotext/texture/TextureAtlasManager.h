#pragma once

#include "chronotext/texture/TextureAtlas.h"

class TextureAtlasManager
{
    std::map<int, TextureAtlas*> atlases;
    bool unloaded;

public:
    TextureAtlasManager() : unloaded(false) {}
    ~TextureAtlasManager();
    
    void setAtlas(const std::string &resourceName, int index, bool useMipmap = false);
    void setAtlas(InputSourceRef inputSource, int index, bool useMipmap = false);

    TextureAtlas* getAtlas(int index);

    void beginTexture(int index);
    void endTexture();
    
    void unload();
    void reload();
};
