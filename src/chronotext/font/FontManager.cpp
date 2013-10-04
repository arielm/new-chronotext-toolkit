#include "chronotext/font/FontManager.h"

using namespace ci;
using namespace std;
using namespace chr;

namespace chronotext
{
    FontManager::~FontManager()
    {
        for (auto font : cache)
        {
            delete font;
        }
    }
    
    XFont* FontManager::getFont(const string &resourceName, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    {
        return getFont(InputSource::getResource(resourceName), useMipmap, useAnisotropy, maxDimensions, slotCapacity);
    }
    
    XFont* FontManager::getFont(InputSourceRef inputSource, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    {
        XFont *font = getFromCache(inputSource, useMipmap, useAnisotropy, maxDimensions, slotCapacity);
        
        if (!font)
        {
            font = new XFont(inputSource, useMipmap, useAnisotropy, maxDimensions, slotCapacity);
            putInCache(font);
        }
        
        return font;
    }
    
    bool FontManager::remove(XFont *font)
    {
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (font == *it)
            {
                delete *it;
                cache.erase(it);
                
                return true;
            }
        }
        
        return false;
    }
    
    void FontManager::clear()
    {
        for (auto font : cache)
        {
            delete font;
        }
        
        cache.clear();
    }
    
    void FontManager::unload()
    {
        for (auto font : cache)
        {
            font->unload();
        }
    }
    
    void FontManager::reload()
    {
        for (auto font : cache)
        {
            font->reload();
        }
    }
    
    XFont* FontManager::getFromCache(InputSourceRef inputSource, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    {
        for (auto font : cache)
        {
            if ((font->inputSource->getUniqueName() == inputSource->getUniqueName()) && (font->useMipmap == useMipmap) && (font->useAnisotropy == useAnisotropy) && (font->maxDimensions == maxDimensions) && (font->slotCapacity == slotCapacity))
            {
                return font;
            }
        }
        
        return NULL;
    }
    
    void FontManager::putInCache(XFont *font)
    {
        cache.push_back(font);
    }
}
