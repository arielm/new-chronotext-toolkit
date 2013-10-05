/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureManager.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    TextureRef TextureManager::getTexture(const string &resourceName, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
    {
        return getTexture(InputSource::getResource(resourceName), useMipmap, flags, wrapS, wrapT);
    }
    
    TextureRef TextureManager::getTexture(InputSourceRef inputSource, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
    {
        return getTexture(TextureRequest(inputSource, useMipmap, flags, wrapS, wrapT));
    }
    
    TextureRef TextureManager::getTexture(const TextureRequest &textureRequest)
    {
        TextureRef texture = getFromCache(textureRequest);
        
        if (!texture)
        {
            texture = make_shared<Texture>(textureRequest);
            putInCache(texture);
        }
        
        return texture;
    }
    
    bool TextureManager::remove(TextureRef texture)
    {
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (texture == *it)
            {
                cache.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    void TextureManager::clear()
    {
        cache.clear();
    }
    
    void TextureManager::unload()
    {
        if (!unloaded)
        {
            unloaded = true;
            
            for (auto texture : cache)
            {
                texture->unload();
            }
        }
    }
    
    void TextureManager::reload()
    {
        if (unloaded)
        {
            unloaded = false;
            
            for (auto texture : cache)
            {
                texture->reload();
            }
        }
    }
    
    TextureRef TextureManager::getFromCache(const TextureRequest &textureRequest)
    {
        for (auto texture : cache)
        {
            if (textureRequest == texture->request)
            {
                return texture;
            }
        }
        
        return TextureRef();
    }
    
    void TextureManager::putInCache(TextureRef texture)
    {
        cache.push_back(texture);
    }
}
