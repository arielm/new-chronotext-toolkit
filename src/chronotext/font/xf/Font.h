/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/QuadMatrix.h"
#include "chronotext/font/xf/FontSequence.h"

#include <map>

namespace chr
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
            enum Alignment
            {
                ALIGN_MIDDLE,
                ALIGN_LEFT,
                ALIGN_RIGHT,
                ALIGN_TOP,
                ALIGN_BASELINE,
                ALIGN_BOTTOM
            };
            
            struct Properties
            {
                bool useAnisotropy;
                int slotCapacity;
                
                Properties(bool useAnisotropy, int slotCapacity)
                :
                useAnisotropy(useAnisotropy),
                slotCapacity(slotCapacity)
                {}
                
                Properties& setCapacity(int slotCapacity)
				{
                    this->slotCapacity = slotCapacity;
                    return *this;
                }
                
                bool operator<(const Properties &rhs) const
                {
                    return std::tie(useAnisotropy, slotCapacity) < std::tie(rhs.useAnisotropy, rhs.slotCapacity);
                }
            };
            
            static Properties Properties2d()
            {
                return Properties(false, 4096);
            }
            
            static Properties Properties3d()
            {
                return Properties(true, 4096);
            }
            
            bool isSpace(wchar_t c) const;
            bool isValid(wchar_t c) const;
            int getGlyphIndex(wchar_t c) const;
            std::wstring getCharacters() const;
            
            void setSize(float size);
            void setMiddleLineFactor(float factor); // DEFAULT-VALUE IS 0, OTHERWISE getOffsetY() FOR "ALIGN_MIDDLE" WILL RETURN middleLineFactor * (getAscent() - getDescent())
            void setDirection(float direction);
            void setAxis(const ci::Vec2f &axis);
            inline void setAxis(float x, float y) { setAxis(ci::Vec2f(x, y)); }
            void setColor(const ci::ColorA &color);
            void setColor(float r, float g, float b, float a);
            
            void setClip(const ci::Rectf &clipRect);
            void setClip(float x1, float y1, float x2, float y2);
            void clearClip(); // INVOKED UPON SEQUENCE-BEGIN
            
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
            float getLineThickness() const;
            float getUnderlineOffset() const;
            float getStrikethroughOffset() const;
            
            float getOffsetX(const std::wstring &text, Alignment align) const;
            float getOffsetY(Alignment align) const; // FOR "ALIGN_MIDDLE": getStrikethroughOffset() WILL BE USED, UNLESS setMiddleLineFactor() HAS BEEN INVOKED
            inline ci::Vec2f getOffset(const std::wstring &text, Alignment alignX, Alignment alignY) const { return ci::Vec2f(getOffsetX(text, alignX), getOffsetY(alignY)); }
            
            QuadMatrix* getMatrix();
            const uint16_t* getIndices() const;
            
            void beginSequence(FontSequence *sequence, bool useColor = false);
            inline void beginSequence(FontSequence &sequence, bool useColor = false) { beginSequence(&sequence, useColor); }
            inline void beginSequence(bool useColor = false) { beginSequence(nullptr, useColor); }
            void endSequence();
            void replaySequence(FontSequence *sequence);
            inline void replaySequence(FontSequence &sequence) { replaySequence(&sequence); }
            
            void addGlyph(int glyphIndex, float x, float y, float z = 0);
            void addTransformedGlyph(int glyphIndex, float x = 0, float y = 0);
            
            friend class FontManager;
            
        protected:
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
            
            FontTexture *texture;
            
            Properties properties;
            const std::vector<uint16_t> &indices;
            QuadMatrix matrix;
            
            float anisotropy;
            
            float size;
            float sizeRatio;
            float middleLineFactor;
            float direction;
            ci::Vec2f axis;
            ci::ColorA color;
            
            bool hasClip;
            ci::Rectf clipRect;
            
            int began;
            bool sequenceUseColor;
            FontSequence *sequence;
            std::unique_ptr<QuadBatch> batch;
            
            Font(FontManager &fontManager, FontData *data, FontTexture *texture, const Properties &properties);
            
            void begin(bool useColor = false);
            void end(bool useColor = false);

            void incrementSequence();
            bool fillQuad(Quad &quad, int glyphIndex, float x, float y) const;
            bool clipQuad(Quad &quad) const;
        };
    }
}
