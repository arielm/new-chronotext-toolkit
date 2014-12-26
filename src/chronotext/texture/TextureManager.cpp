/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "TextureManager.h"

using namespace std;
using namespace ci;

namespace chr
{
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
        
        auto texture = make_shared<Texture>(textureRequest);
        textures[textureRequest] = texture;
        
        return texture;
    }
    
    void TextureManager::discardTexture(Texture::Ref texture)
    {
        if (texture)
        {
            texture->discard();
        }
    }

    bool TextureManager::reloadTexture(Texture::Ref texture)
    {
        if (texture)
        {
            return texture->reload();
        }
        
        return false;
    }
    
    void TextureManager::discardTextures()
    {
        for (auto &element : textures)
        {
            element.second->discard();
        }
    }
    
    void TextureManager::reloadTextures()
    {
        for (auto &element : textures)
        {
            element.second->reload();
        }
    }
}
