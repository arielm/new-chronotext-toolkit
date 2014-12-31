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
    atomic<bool> TextureManager::LOG_VERBOSE (false);
    atomic<bool> TextureManager::PROBE_MEMORY (false);

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
    
    Texture::Ref TextureManager::findTexture(const Texture::Request &request) const
    {
        auto it = textures.find(request);
        
        if (it != textures.end())
        {
            return it->second;
        }
        
        return nullptr;
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
            if ((tag < 0) || (tag == element.second->request.tag))
            {
                element.second->discard();
            }
        }
    }
    
    void TextureManager::reloadTextures(int tag)
    {
        for (auto &element : textures)
        {
            if ((tag < 0) || (tag == element.second->request.tag))
            {
                element.second->reload();
            }
        }
    }
}
