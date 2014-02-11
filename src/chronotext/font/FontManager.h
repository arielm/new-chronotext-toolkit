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
    
    struct FontData
    {
        int glyphCount;
        std::map<wchar_t, int> glyphs;
        
        float nativeFontSize;
        float height;
        float ascent;
        float descent;
        float spaceAdvance;
        float strikethroughFactor;
        float underlineOffset;
        float lineThickness;
        
        float *w;
        float *h;
        float *le;
        float *te;
        float *advance;
        
        float *u1;
        float *v1;
        float *u2;
        float *v2;
        
        FontData(int glyphCount);
        ~FontData();
    };
    
    struct FontAtlas
    {
        int width;
        int height;
        unsigned char *data;
        
        FontAtlas(int width, int height);
        ~FontAtlas();
        
        void addGlyph(unsigned char *glyphData, int x, int y, int glyphWidth, int glyphHeight);
    };
    
    struct FontTexture
    {
        int width;
        int height;
        GLuint name;
        
        InputSourceRef inputSource;
        bool useMipmap;
        
        FontTexture(FontAtlas *atlas, InputSourceRef inputSource, bool useMipmap);
        ~FontTexture();
        
        void upload(FontAtlas *atlas);
        void discard();
    };
    
    class FontManager
    {
    public:
        XFont* getFont(const std::string &resourceName, const XFont::Properties &properties);
        XFont* getFont(InputSourceRef inputSource, const XFont::Properties &properties);
        
        /*
        bool remove(XFont *font);
        void clear();
        */
        
        void discardTextures();
        void reloadTextures();
        
    protected:
        std::map<FontKey, std::unique_ptr<XFont>> fonts;
        std::map<std::string, std::unique_ptr<FontData>> fontData;
        std::map<std::pair<std::string, bool>, std::unique_ptr<FontTexture>> textures;

        static std::pair<FontData*, FontAtlas*> fetchFont(InputSourceRef source);
    };
}

namespace chr = chronotext;
