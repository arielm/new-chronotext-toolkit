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
        bool useAnisotropy;
        int maxDimensions;
        int slotCapacity;
        
        FontKey(const std::string &uri, bool useAnisotropy, int maxDimensions, int slotCapacity)
        :
        uri(uri),
        useAnisotropy(useAnisotropy),
        maxDimensions(maxDimensions),
        slotCapacity(slotCapacity)
        {}
        
        bool operator<(const FontKey &rhs) const
        {
            return std::tie(uri, useAnisotropy, maxDimensions, slotCapacity) < std::tie(rhs.uri, rhs.useAnisotropy, rhs.maxDimensions, rhs.slotCapacity);
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
        
        void addGlyph(unsigned char *glyphData, int glyphX, int glyphY, int glyphWidth, int glyphHeight);
    };
    
    struct FontTexture
    {
        int width;
        int height;
        GLuint name;
        InputSourceRef inputSource;
        
        FontTexture(FontAtlas *atlas, InputSourceRef inputSource);
        ~FontTexture();
        
        void upload(FontAtlas *atlas);
        void discard();
        void reload();
    };
    
    class FontManager
    {
    public:
        std::shared_ptr<XFont> getCachedFont(InputSourceRef inputSource, const XFont::Properties &properties);
        
        void unload(std::shared_ptr<XFont> font);
        void unload(InputSourceRef inputSource);
        void unload();
        
        void discardTextures();
        void reloadTextures();
        
        static std::pair<FontData*, FontAtlas*> fetchFontDataAndAtlas(InputSourceRef source);
        
    protected:
        std::map<FontKey, std::shared_ptr<XFont>> fonts;
        std::map<std::string, std::pair<std::unique_ptr<FontData>, std::unique_ptr<FontTexture>>> fontDataAndTextures;
        std::vector<GLushort> indices;

        const std::vector<GLushort>& getIndices(int capacity); // REMINDER: indices.data() WILL RETURN A DIFFERENT ADDRESS FOR EACH NEW CAPACITY
        void discardUnusedTextures();
    };
}

namespace chr = chronotext;
