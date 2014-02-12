/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * REACHING A JUNCTION WHERE SOME IMPORTANT DESIGN DECISION MUST BE TAKEN...
 *
 *
 * OPEN QUESTIONS:
 *
 * 1) SHOULD FontManager::getFont(...) RETURN A shared_ptr?
 *
 * 2) SHOULD TEXTURES-ATLASES BE "AUTOMATICALLY RELOADABLE"?
 *    E.G. FontManager::discardTextures() IS CALLED, BUT NOT FOLLOWED BY FontManager::reloadTextures()
 *
 * 3) IN ADDITION, WE NEED TO HANDLE THE FOLLOWING IN FontManager:
 *    bool remove(XFont *font);
 *    void clear();
 *
 *
 * CURRENTLY, THE FACT THAT WE CAN USE MIPMAP AND NON-MIPMAP VERSIONS OF THE SAME ATLAS
 * IS MAKING THE IMPLEMENTATION OF 1) AND 2) VERY COMPLICATED
 *
 * ON THE OTHER-HAND, WE DON'T REALLY NEED THE NON-MIPMAP FEATURE (TO BE USED FOR CRIP TEXT)
 *
 * TO BE MORE EXACT: THE FEATURE WILL BE PROPERLY ADDRESSED BY THE FORTHCOMING ZFont SYSTEM,
 * WHERE, AMONG OTHER THINGS, IT IS POSSIBLE TO GENERATE GLYPHS ON-THE-FLY ACCORDING THE
 * RESOLUTION OF THE TARGET-DEVICE...
 *
 *
 * CONCLUSION: LET'S MAKE XFont MIPMAP-ONLY!
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
        XFont* getFont(const std::string &resourceName, const XFont::Properties &properties);
        XFont* getFont(InputSourceRef inputSource, const XFont::Properties &properties);
        
        /*
        bool remove(XFont *font);
        void clear();
        */
        
        void discardTextures();
        void reloadTextures();
        
        static std::pair<FontData*, FontAtlas*> fetchFontDataAndAtlas(InputSourceRef source);
        
    protected:
        std::map<FontKey, std::unique_ptr<XFont>> fonts;
        std::map<std::string, std::pair<std::unique_ptr<FontData>, std::unique_ptr<FontTexture>>> fontDataAndTextures;
        std::vector<GLushort> indices;

        const std::vector<GLushort>& getIndices(int capacity); // REMINDER: indices.data() WILL RETURN A DIFFERENT ADDRESS FOR EACH NEW CAPACITY
    };
}

namespace chr = chronotext;
