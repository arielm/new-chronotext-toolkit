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
            
            for (auto it = cache.begin(); it != cache.end(); ++it)
            {
                (*it)->unload();
            }
        }
    }
    
    void TextureManager::reload()
    {
        if (unloaded)
        {
            unloaded = false;
            
            for (auto it = cache.begin(); it != cache.end(); ++it)
            {
                (*it)->reload();
            }
        }
    }
    
    TextureRef TextureManager::getFromCache(const TextureRequest &textureRequest)
    {
        for (auto it = cache.cbegin(); it != cache.cend(); ++it)
        {
            TextureRef texture = *it;
            
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
