#include "chronotext/font/FontManager.h"

using namespace ci;
using namespace std;
using namespace chr;

namespace chronotext
{
    FontManager::~FontManager()
    {
        for (list<XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
        {
            delete *it;
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
        for (list<XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
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
        for (list<XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
        {
            delete *it;
        }
        
        cache.clear();
    }
    
    void FontManager::unload()
    {
        for (list<XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
        {
            (*it)->unload();
        }
    }
    
    void FontManager::reload()
    {
        for (list<XFont*>::iterator it = cache.begin(); it != cache.end(); ++it)
        {
            (*it)->reload();
        }
    }
    
    XFont* FontManager::getFromCache(InputSourceRef inputSource, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    {
        for (list<XFont*>::const_iterator it = cache.begin(); it != cache.end(); ++it)
        {
            XFont *font = *it;
            
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
