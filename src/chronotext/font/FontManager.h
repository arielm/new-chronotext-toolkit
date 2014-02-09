/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/XFont.h"

namespace chronotext
{
    struct FontKey
    {
        std::string uri;
        bool useMipmap;
        bool useAnisotropy;
        int maxDimensions;
        int slotCapacity;
        
        FontKey(const std::string &uri, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
        :
        uri(uri),
        useMipmap(useMipmap),
        useAnisotropy(useAnisotropy),
        maxDimensions(maxDimensions),
        slotCapacity(slotCapacity)
        {}
        
        bool operator<(const FontKey &rhs) const
        {
            return std::tie(useMipmap, useAnisotropy, maxDimensions, slotCapacity, uri) < std::tie(rhs.useMipmap, rhs.useAnisotropy, rhs.maxDimensions, rhs.slotCapacity, rhs.uri);
        }
    };
    
    class FontManager
    {
    public:
        XFont* getFont(const std::string &resourceName, const XFont::Properties &properties);
        XFont* getFont(InputSourceRef inputSource, const XFont::Properties &properties);
        
        bool remove(chr::XFont *font);
        void clear();
        
        void unload();
        void reload();
        
    protected:
        std::map<FontKey, std::unique_ptr<XFont>> cache;
    };
}

namespace chr = chronotext;
