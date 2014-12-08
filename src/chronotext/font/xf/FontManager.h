/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/font/xf/Font.h"

namespace chr
{
    namespace xf
    {
        struct FontData
        {
            int glyphCount;
            std::map<wchar_t, int> glyphs;
            
            float baseSize;
            float height;
            float ascent;
            float descent;
            float lineThickness;
            float underlineOffset;
            float strikethroughOffset;
            float spaceAdvance;
            
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
            uint32_t id;
            InputSource::Ref inputSource;
            
            FontTexture(FontAtlas *atlas, InputSource::Ref inputSource);
            ~FontTexture();
            
            void upload(FontAtlas *atlas);
            void discard();
            void reload();
            void bind();
            
            size_t getMemoryUsage() const;
            inline ci::Vec2i getSize() const { return ci::Vec2i(width, height); }
        };
        
        class FontManager
        {
        public:
            std::shared_ptr<Font> getCachedFont(InputSource::Ref inputSource, const Font::Properties &properties);
            
            void unload(std::shared_ptr<Font> font);
            void unload(InputSource::Ref inputSource);
            void unload();
            
            void discardTextures();
            void reloadTextures();
            
            /*
             * RETURNS THE MEMORY USED BY ALL THE GLYPH TEXTURE-ATLASES
             * CURRENTLY: "ALPHA" TEXTURES ARE USED (ONE BYTE PER PIXEL)
             * NOTE THAT THE GPU MAY DECIDE TO USE MORE MEMORY INTERNALLY
             */
            size_t getTextureMemoryUsage() const;
            
            friend class Font;
            friend struct FontTexture;
            
        protected:
            std::map<std::pair<std::string, Font::Properties>, std::shared_ptr<Font>> fonts;
            std::map<std::string, std::pair<std::unique_ptr<FontData>, std::unique_ptr<FontTexture>>> fontDataAndTextures;
            std::vector<uint16_t> indices;
            
            void discardUnusedTextures();
            static std::pair<FontData*, FontAtlas*> fetchFontDataAndAtlas(InputSource::Ref source);
            
            const std::vector<uint16_t>& getIndices(int capacity);
        };
    }
}
