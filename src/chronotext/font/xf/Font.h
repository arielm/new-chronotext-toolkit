/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/xf/FontSequence.h"

#include "cinder/gl/gl.h"

#include <map>

namespace chronotext
{
    typedef class xf::Font XFont;

    namespace xf
    {
        class FontManager;
        struct FontData;
        struct FontTexture;
        
        class Font
        {
        public:
            typedef enum
            {
                ALIGN_MIDDLE,
                ALIGN_LEFT,
                ALIGN_RIGHT,
                ALIGN_TOP,
                ALIGN_BASELINE,
                ALIGN_BOTTOM
            }
            Alignment;
            
            struct Properties
            {
                bool useAnisotropy;
                int slotCapacity;
                
                Properties(bool useAnisotropy, int slotCapacity)
                :
                useAnisotropy(useAnisotropy),
                slotCapacity(slotCapacity)
                {
                    assert((slotCapacity > 0) && (slotCapacity <= 8192));
                }
                
                bool operator<(const Properties &rhs) const
                {
                    return std::tie(useAnisotropy, slotCapacity) < std::tie(rhs.useAnisotropy, rhs.slotCapacity);
                }
            };
            
            static Properties Properties2d(int slotCapacity = 1024)
            {
                return Properties(false, slotCapacity);
            }
            
            static Properties Properties3d(int slotCapacity = 4096)
            {
                return Properties(true, slotCapacity);
            }
            
            bool isSpace(wchar_t c) const;
            bool isValid(wchar_t c) const;
            int getGlyphIndex(wchar_t c) const;
            std::wstring getCharacters() const;
            
            void setSize(float size);
            void setDirection(float direction);
            void setAxis(const ci::Vec2f &axis);
            void setColor(const ci::ColorA &color);
            void setColor(float r, float g, float b, float a);
            void setStrikethroughFactor(float factor);
            
            void setClip(const ci::Rectf &clipRect);
            void setClip(float x1, float y1, float x2, float y2);
            void clearClip();
            
            float getSize() const;
            float getDirection() const;
            const ci::Vec2f& getAxis() const;
            
            float getGlyphAdvance(int glyphIndex) const;
            float getCharAdvance(wchar_t c) const;
            float getStringAdvance(const std::wstring &s) const;
            float getSubStringAdvance(const std::wstring &s, int begin, int end) const;
            
            float getHeight() const;
            float getAscent() const;
            float getDescent() const;
            float getStrikethroughOffset() const;
            float getUnderlineOffset() const;
            float getLineThickness() const;
            
            float getOffsetX(const std::wstring &text, Alignment align) const;
            float getOffsetY(Alignment align) const;
            inline ci::Vec2f getOffset(const std::wstring &text, Alignment alignX, Alignment alignY) const { return ci::Vec2f(getOffsetX(text, alignX), getOffsetY(alignY)); }
            
            FontMatrix* getMatrix();
            const GLushort* getIndices() const;
            
            void beginSequence(FontSequence *sequence, bool useColor = false);
            inline void beginSequence(bool useColor = false) { beginSequence(nullptr, useColor); }
            void endSequence();
            void replaySequence(FontSequence *sequence);
            
            void addGlyph(int glyphIndex, float x, float y, float z = 0);
            void addTransformedGlyph(int glyphIndex, float x, float y);
            
            friend class FontManager;
            
        protected:
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
            
            FontTexture *texture;
            
            Properties properties;
            const std::vector<GLushort> &indices;
            FontMatrix matrix;
            
            bool anisotropyAvailable;
            float maxAnisotropy;
            
            float size;
            float sizeRatio;
            float direction;
            ci::Vec2f axis;
            ci::ColorA color;
            
            bool hasClip;
            ci::Rectf clipRect;
            
            int began;
            bool sequenceUseColor;
            FontSequence *sequence;
            std::unique_ptr<GlyphBatch> batch;
            
            Font(FontManager &fontManager, FontData *data, FontTexture *texture, const Properties &properties);
            
            void begin(bool useColor = false);
            void end(bool useColor = false);

            void incrementSequence();
            GlyphQuad obtainQuad(int glyphIndex, float x, float y) const;
            bool clipQuad(GlyphQuad &quad) const;
        };
    }
}

namespace chr = chronotext;
