/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/XFontSequence.h"

#include "cinder/gl/gl.h"

#include <map>

namespace chronotext
{
    struct FontData;
    struct FontTexture;

    class XFont
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
            int maxDimensions;
            int slotCapacity;
            
            Properties(bool useAnisotropy, int maxDimensions, int slotCapacity)
            :
            useAnisotropy(useAnisotropy),
            maxDimensions(maxDimensions),
            slotCapacity(slotCapacity)
            {}
            
            static Properties Default2D(int slotCapacity = 1024)
            {
                return Properties(false, 2, slotCapacity);
            }
            
            static Properties Default3D(int slotCapacity = 2048)
            {
                return Properties(true, 3, slotCapacity);
            }
        };
        
        ~XFont(); // MUST BE PUBLIC BECAUSE OF unique_ptr

        bool isSpace(wchar_t c) const;
        bool isValid(wchar_t c) const;
        int getGlyphIndex(wchar_t c) const;
        std::wstring getCharacters() const;
        
        void setSize(float size);
        void setDirection(float direction);
        void setAxis(const ci::Vec2f &axis);
        void setStrikethroughFactor(float factor);

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
        
        void begin();
        void end();
        
        void beginSequence(XFontSequence *sequence, int dimensions);
        void endSequence();
        
        void addGlyph(int glyphIndex, float x, float y);
        void addGlyph(int glyphIndex, float x, float y, const ci::Rectf &clip);

        void addGlyph(int glyphIndex, float x, float y, float z);
        void addGlyph(int glyphIndex, float x, float y, float z, const ci::Rectf &clip);
        
        void addTransformedGlyph2D(int glyphIndex, float x, float y);
        void addTransformedGlyph2D(int glyphIndex, float x, float y, const ci::Rectf &clip);
        
        void addTransformedGlyph3D(int glyphIndex, float x, float y);
        void addTransformedGlyph3D(int glyphIndex, float x, float y, const ci::Rectf &clip);
        
        friend class FontManager;
        
    protected:
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
        
        int textureWidth;
        int textureHeight;
        FontTexture *texture;
        
        Properties properties;
        const std::vector<GLushort> &indices;
        float *vertices;
        
        bool anisotropyAvailable;
        float maxAnisotropy;
        
        float size;
        float sizeRatio;
        float direction;
        ci::Vec2f axis;

        int began;
        FontMatrix matrix;
        
        int sequenceSize;
        int sequenceDimensions;
        float *sequenceVertices;
        XFontSequence *sequence;
        
        XFont(FontData *data, FontTexture *texture, const std::vector<GLushort> &indices, const Properties &properties);
        
        void flush(int count);
        void incrementSequence();
        
        GlyphQuad getGlyphQuad(int glyphIndex, float x, float y) const;
        bool computeClip(GlyphQuad &quad, const ci::Rectf &clip);
        
        static int addQuad(const GlyphQuad &quad, float *vertices);
        static int addQuad(const GlyphQuad &quad, float z, float *vertices);
    };
}

namespace chr = chronotext;
