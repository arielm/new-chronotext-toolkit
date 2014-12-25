/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/GlyphData.h"
#include "chronotext/font/zf/FontTexture.h"
#include "chronotext/quad/QuadMatrix.h"
#include "chronotext/InputSource.h"

#include "hb.h"

#include <map>
#include <set>

namespace chr
{
    namespace zf
    {
        struct Shape;
        
        class ActualFont
        {
        public:
            struct Descriptor
            {
                InputSource::Ref source;
                int faceIndex;
                float scale;
                bool forceMemoryLoad;
                
                Descriptor()
                {}
                
                Descriptor(InputSource::Ref source, int faceIndex = 0, float scale = 1, bool forceMemoryLoad = false)
                :
                source(source),
                faceIndex(faceIndex),
                scale(scale),
                forceMemoryLoad(forceMemoryLoad)
                {}
                
                bool empty()
                {
                    return !bool(source);
                }
            };
            
            struct Key
            {
                std::string uri;
                int faceIndex;
                float baseSize;
                bool useMipmap;
                
                Key(const Descriptor &descriptor, float baseSize, bool useMipmap)
                :
                uri(descriptor.source->getURI()),
                faceIndex(descriptor.faceIndex),
                baseSize(baseSize * descriptor.scale),
                useMipmap(useMipmap)
                {}
                
                bool operator<(const Key &rhs) const
                {
                    return tie(uri, faceIndex, baseSize, useMipmap) < tie(rhs.uri, rhs.faceIndex, rhs.baseSize, rhs.useMipmap);
                }
            };
            
            struct Glyph
            {
                FontTexture *texture;
                ci::Vec2f offset;
                ci::Vec2f size;
                
                float u1;
                float v1;
                float u2;
                float v2;
                
                Glyph()
                :
                texture(nullptr)
                {}
                
                Glyph(FontTexture *texture, ci::Vec2f offset, ci::Vec2f size)
                :
                texture(texture),
                offset(offset),
                size(size)
                {
                    u1 = 0;
                    v1 = 0;
                    u2 = size.x / texture->width;
                    v2 = size.y / texture->height;
                }
            };
            
            struct Metrics
            {
                float height;
                float ascent;
                float descent;
                float underlineOffset;
                float strikethroughOffset;
                float lineThickness;
                
                Metrics& operator*(float s)
                {
                    height *= s;
                    ascent *= s;
                    descent *= s;
                    lineThickness *= s;
                    underlineOffset *= s;
                    strikethroughOffset *= s;
                    
                    return *this;
                }
            };
            
            ~ActualFont(); // MUST BE PUBLIC BECAUSE OF unique_ptr

            bool isSpace(hb_codepoint_t codepoint) const;
            hb_codepoint_t getCodepoint(FT_ULong charCode) const;
            std::string getFullName() const;

        protected:
            std::shared_ptr<FreetypeHelper> ftHelper;
            
            Descriptor descriptor;
            float baseSize;
            bool useMipmap;
            int padding;
            
            ci::Vec2f scale;
            Metrics metrics;
            bool loaded;
            
            ci::Buffer memoryBuffer;
            FT_Face ftFace;
            hb_font_t *hbFont;
            
            std::map<hb_codepoint_t, std::unique_ptr<Glyph>> glyphs;
            std::vector<std::unique_ptr<FontTexture>> textures;
            std::set<hb_codepoint_t> spaceSeparators;
            
            ActualFont(std::shared_ptr<FreetypeHelper> ftHelper, const Descriptor &descriptor, float baseSize, bool useMipmap);
            
            void reload();
            void unload();
            void discardTextures();
            void reloadTextures();
            size_t getTextureMemoryUsage() const;
            
            Glyph* fillQuad(Quad &quad, const Shape &shape, const ci::Vec2f &position, float sizeRatio);
            Glyph* getGlyph(hb_codepoint_t codepoint);
            Glyph* createGlyph(hb_codepoint_t codepoint);
            void reloadTexture(FontTexture *texture);
            
            friend class FontManager;
            friend class VirtualFont;
            friend class FontTexture;
        };
    }
}
