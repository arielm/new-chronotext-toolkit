#include "chronotext/texture/TextureManager.h"

using namespace std;
using namespace ci;

TextureManager::~TextureManager()
{
    for (list<Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
    {
        delete *it;
    }
}

Texture* TextureManager::getFromCache(InputSourceRef inputSource, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT)
{
    for (list<Texture*>::const_iterator it = cache.begin(); it != cache.end(); ++it)
    {
        Texture *texture = *it;

        if ((texture->inputSource->getUniqueName() == inputSource->getUniqueName()) && (texture->useMipmap == useMipmap) && (texture->filter == filter) && (texture->wrapS == wrapS) && (texture->wrapT == wrapT))
        {
            return texture;
        }
    }
    
    return NULL;
}

void TextureManager::putInCache(Texture *texture)
{
    cache.push_back(texture);
}

Texture* TextureManager::getTexture(const string &resourceName, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT)
{
    return getTexture(InputSource::getResource(resourceName), useMipmap, filter, wrapS, wrapT);
}

Texture* TextureManager::getTexture(InputSourceRef inputSource, bool useMipmap, int filter, GLenum wrapS, GLenum wrapT)
{
    Texture *texture = getFromCache(inputSource, useMipmap, filter, wrapS, wrapT);
    
    if (!texture)
    {
        texture = new Texture(inputSource, useMipmap, filter, wrapS, wrapT);
        putInCache(texture);
    }
    
    return texture;
}

bool TextureManager::remove(Texture *texture)
{
    for (list<Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
    {
        if (texture == *it)
        {
            delete *it;
            cache.erase(it);

            return true;
        }
    }
    
    return false;
}

void TextureManager::clear()
{
    for (list<Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
    {
        delete *it;
    }

    cache.clear();
}

void TextureManager::unload()
{
    if (!unloaded)
    {
        unloaded = true;
        
        for (list<Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
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
        
        for (list<Texture*>::iterator it = cache.begin(); it != cache.end(); ++it)
        {
            (*it)->reload();
        }
    }
}
