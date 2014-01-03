/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/XFont.h"

/*
 * TODO:
 * A BETTER VERSION SHOULD RETURN A "DERIVED" FONT WHENEVER POSSIBLE
 * E.G. WHEN ASKING FOR A FONT ALREADY DEFINED, BUT WITH DIFFERENT PARAMETERS (useAnisotropy, maxDimensions OR slotCapacity)
 */

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
            return tie(useMipmap, useAnisotropy, maxDimensions, slotCapacity, uri) < tie(rhs.useMipmap, rhs.useAnisotropy, rhs.maxDimensions, rhs.slotCapacity, rhs.uri);
        }
    };
    
    class FontManager
    {
    public:
        chr::XFont* getFont(const std::string &resourceName, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
        chr::XFont* getFont(InputSourceRef inputSource, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
        
        bool remove(chr::XFont *font);
        void clear();
        
        void unload();
        void reload();
        
    protected:
        std::map<FontKey, std::unique_ptr<XFont>> cache;
    };
}

namespace chr = chronotext;
