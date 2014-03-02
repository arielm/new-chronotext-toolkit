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
    TextureRef TextureManager::getTexture(const string &resourceName, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    TextureRef TextureManager::getTexture(InputSourceRef inputSource, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    TextureRef TextureManager::getTexture(const TextureRequest &textureRequest)
    {
        auto it = textures.find(textureRequest);
        
        if (it != textures.end())
        {
            return it->second;
        }
        else
        {
            auto texture = make_shared<Texture>(textureRequest);
            textures[textureRequest] = texture;
            
            return texture;
        }
    }
    
    bool TextureManager::remove(TextureRef texture)
    {
        for (auto it = textures.begin(); it != textures.end(); ++it)
        {
            if (it->second == texture)
            {
                textures.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    void TextureManager::clear()
    {
        textures.clear();
    }
    
    void TextureManager::discard()
    {
        for (auto &it : textures)
        {
            it.second->discard();
        }
    }
    
    void TextureManager::reload()
    {
        for (auto &it : textures)
        {
            it.second->reload();
        }
    }
}
