/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureManager.h"

using namespace std;
using namespace ci;

namespace chr
{
    Texture::Ref TextureManager::getTexture(const string &resourceName, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    Texture::Ref TextureManager::getTexture(InputSource::Ref inputSource, bool useMipmap, TextureRequest::Flags flags)
    {
        return getTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    Texture::Ref TextureManager::getTexture(const TextureRequest &textureRequest)
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
    
    bool TextureManager::remove(Texture::Ref texture)
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
