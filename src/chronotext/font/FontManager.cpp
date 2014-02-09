/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/FontManager.h"

using namespace ci;
using namespace std;
using namespace chr;

namespace chronotext
{
    XFont* FontManager::getFont(const string &resourceName, const XFont::Properties &properties)
    {
        return getFont(InputSource::getResource(resourceName), properties);
    }
    
    XFont* FontManager::getFont(InputSourceRef inputSource, const XFont::Properties &properties)
    {
        FontKey key(inputSource->getURI(), properties.useMipmap, properties.useAnisotropy, properties.maxDimensions, properties.slotCapacity);
        auto it = cache.find(key);
        
        if (it != cache.end())
        {
            return it->second.get();
        }
        else
        {
            auto font = new XFont(inputSource, properties);
            cache[key] = unique_ptr<XFont>(font);
            
            return font;
        }
    }
    
    bool FontManager::remove(XFont *font)
    {
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (it->second.get() == font)
            {
                cache.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    void FontManager::clear()
    {
        cache.clear();
    }
    
    void FontManager::unload()
    {
        for (auto &it : cache)
        {
            it.second->unload();
        }
    }
    
    void FontManager::reload()
    {
        for (auto &it : cache)
        {
            it.second->reload();
        }
    }
}
