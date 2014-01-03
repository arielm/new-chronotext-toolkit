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
    TextureRef TextureManager::getTexture(const string &resourceName, bool useMipmap, int flags)
    {
        return getTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    TextureRef TextureManager::getTexture(InputSourceRef inputSource, bool useMipmap, int flags)
    {
        return getTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    TextureRef TextureManager::getTexture(const TextureRequest &textureRequest)
    {
        auto it = cache.find(textureRequest);
        
        if (it != cache.end())
        {
            return it->second;
        }
        else
        {
            auto texture = make_shared<Texture>(textureRequest);
            cache[textureRequest] = texture;
            
            return texture;
        }
    }
    
    bool TextureManager::remove(TextureRef texture)
    {
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (it->second == texture)
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
            
            for (auto &it : cache)
            {
                it.second->unload();
            }
        }
    }
    
    void TextureManager::reload()
    {
        if (unloaded)
        {
            unloaded = false;
            
            for (auto &it : cache)
            {
                it.second->reload();
            }
        }
    }
}
