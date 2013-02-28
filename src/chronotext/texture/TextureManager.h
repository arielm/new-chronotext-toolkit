#pragma once

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"

#include <list>

class TextureManager
{
    std::list<Texture*> cache;
    bool unloaded;
    
    Texture* getFromCache(InputSourceRef inputSource, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT);
    void putInCache(Texture *texture);
    
public:
    TextureManager() : unloaded(false) {}
    ~TextureManager();

    Texture* getTexture(const std::string &resourceName, bool useMipmap = false, int filter = TextureHelper::FILTER_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    Texture* getTexture(InputSourceRef inputSource, bool useMipmap = false, int filter = TextureHelper::FILTER_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    
    bool remove(Texture *texture);
    void clear();
    
    void unload();
    void reload();
};
