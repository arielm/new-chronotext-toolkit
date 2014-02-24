/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/font/xf/Font.h"

namespace chronotext
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
            GLuint id;
            InputSourceRef inputSource;
            
            FontTexture(FontAtlas *atlas, InputSourceRef inputSource);
            ~FontTexture();
            
            void upload(FontAtlas *atlas);
            void discard();
            void reload();
            void bind();
            
            inline ci::Vec2i getSize() const { return ci::Vec2i(width, height); }
        };
        
        class FontManager
        {
        public:
            std::shared_ptr<Font> getCachedFont(InputSourceRef inputSource, const Font::Properties &properties);
            
            void unload(std::shared_ptr<Font> font);
            void unload(InputSourceRef inputSource);
            void unload();
            
            void discardTextures();
            void reloadTextures();
            
            friend class Font;
            friend class FontTexture;
            
        protected:
            std::map<std::pair<std::string, Font::Properties>, std::shared_ptr<Font>> fonts;
            std::map<std::string, std::pair<std::unique_ptr<FontData>, std::unique_ptr<FontTexture>>> fontDataAndTextures;
            std::vector<GLushort> indices;
            
            void discardUnusedTextures();
            static std::pair<FontData*, FontAtlas*> fetchFontDataAndAtlas(InputSourceRef source);
            
            const std::vector<GLushort>& getIndices(int capacity);
        };
    }
}

namespace chr = chronotext;
