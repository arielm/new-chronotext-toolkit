/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
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
            return std::tie(uri, useMipmap, useAnisotropy, maxDimensions, slotCapacity) < std::tie(rhs.uri, rhs.useMipmap, rhs.useAnisotropy, rhs.maxDimensions, rhs.slotCapacity);
        }
    };
    
    class FontManager
    {
    public:
        XFont* getFont(const std::string &resourceName, const XFont::Properties &properties);
        XFont* getFont(InputSourceRef inputSource, const XFont::Properties &properties);
        
        /*
        bool remove(chr::XFont *font);
        void clear();
        
        void unload();
        void reload();
        */
        
    protected:
        std::map<FontKey, std::unique_ptr<XFont>> cache;
        std::map<std::string, std::unique_ptr<FontData>> fontData;
        std::map<std::pair<std::string, bool>, std::unique_ptr<FontTexture>> textures;

        FontTexture* uploadTexture(chr::InputSourceRef source, FontAtlas *atlas, bool useMipmap);

        static std::pair<FontData*, FontAtlas*> fetchFont(InputSourceRef source);
        static void addAtlasUnit(FontAtlas *atlas, unsigned char *glyphData, int x, int y, int width, int height);
        static GLuint uploadAtlas(FontAtlas *atlas, bool useMipmap);
    };
}

namespace chr = chronotext;
