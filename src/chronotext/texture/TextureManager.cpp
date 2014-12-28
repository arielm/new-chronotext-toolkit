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
    Texture::Ref TextureManager::getTexture(InputSource::Ref inputSource, bool useMipmap, Texture::Request::Flags flags)
    {
        return getTexture(Texture::Request(inputSource, useMipmap, flags));
    }
    
    Texture::Ref TextureManager::getTexture(const Texture::Request &request)
    {
        auto it = textures.find(request);
        
        if (it != textures.end())
        {
            return it->second;
        }
        
        auto texture = make_shared<Texture>(request);
        textures[request] = texture;
        
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
    
    void TextureManager::discardTextures(int tag)
    {
        for (auto &element : textures)
        {
            if (tag == element.second->request.tag)
            {
                element.second->discard();
            }
        }
    }
    
    void TextureManager::reloadTextures(int tag)
    {
        for (auto &element : textures)
        {
            if (tag == element.second->request.tag)
            {
                element.second->reload();
            }
        }
    }
}
