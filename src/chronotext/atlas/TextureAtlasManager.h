/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/atlas/TextureAtlas.h"

class TextureAtlasManager
{
    std::map<int, TextureAtlas*> atlases;
    bool unloaded;

public:
    TextureAtlasManager() : unloaded(false) {}
    ~TextureAtlasManager();
    
    void setAtlas(const std::string &resourceName, int index, bool useMipmap = false);
    void setAtlas(chr::InputSourceRef inputSource, int index, bool useMipmap = false);

    TextureAtlas* getAtlas(int index);

    void beginTexture(int index);
    void endTexture();
    
    void unload();
    void reload();
};
