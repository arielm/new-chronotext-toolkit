/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/XFontSequence.h"

#include "cinder/gl/gl.h"

#include <map>

namespace chronotext
{
    class XFont
    {
        struct Data
        {
            float nativeFontSize;
            float height;
            float ascent;
            float descent;
            float spaceAdvance;
            float strikethroughFactor;
            float underlineOffset;
            float lineThickness;
            
            int atlasWidth;
            int atlasHeight;
            int atlasPadding;
            int unitMargin;
            int unitPadding;
            
            std::unique_ptr<unsigned char[]> atlasData;
            
            std::unique_ptr<float[]> w;
            std::unique_ptr<float[]> h;
            std::unique_ptr<float[]> le;
            std::unique_ptr<float[]> te;
            std::unique_ptr<float[]> advance;
            
            std::unique_ptr<float[]> u1;
            std::unique_ptr<float[]> v1;
            std::unique_ptr<float[]> u2;
            std::unique_ptr<float[]> v2;
            
            int glyphCount;
            std::map<wchar_t, int> glyphs;
        };
        
        std::unique_ptr<Data> data; // FIXME: SHOULD BE STORED IN FontManager;
        GLuint textureName;

        float nativeFontSize;
        float height;
        float ascent;
        float descent;
        float spaceAdvance;
        float strikethroughFactor;
        float underlineOffset;
        float lineThickness;
        
        int atlasWidth;
        int atlasHeight;
        int atlasPadding;
        int unitMargin;
        int unitPadding;
        
        int glyphCount;
        std::map<wchar_t, int> glyphs;
        
        float *w;
        float *h;
        float *le;
        float *te;
        float *advance;
        
        float *u1;
        float *v1;
        float *u2;
        float *v2;

        bool anisotropyAvailable;
        float maxAnisotropy;

        bool unloaded;
        int began;

        float size;
        float sizeRatio;
        float direction;
        ci::Vec2f axis;
        
        FontMatrix matrix;

        std::vector<GLushort> indices;
        float *vertices;
        
        int sequenceSize;
        int sequenceDimensions;
        float *sequenceVertices;
        XFontSequence *sequence;
        
        static Data* fetchFontData(InputSourceRef source);
        static void addAtlasUnit(unsigned char *srcData, unsigned char *dstData, int atlasWidth, int xx, int yy, int ww, int hh);
        static GLuint uploadAtlasData(unsigned char *atlasData, int atlasWidth, int atlasHeight, bool useMipmap);
        
        void flush(int count);
        void incrementSequence();
        
        GlyphQuad getGlyphQuad(int glyphIndex, float x, float y) const;
        bool computeClip(GlyphQuad &quad, const ci::Rectf &clip);
        
        static int addQuad(const GlyphQuad &quad, float *vertices);
        static int addQuad(const GlyphQuad &quad, float z, float *vertices);
        
    public:
        enum
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
            bool useMipmap;
            bool useAnisotropy;
            int maxDimensions;
            int slotCapacity;
            
            Properties(bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
            :
            useMipmap(useMipmap),
            useAnisotropy(useAnisotropy),
            maxDimensions(maxDimensions),
            slotCapacity(slotCapacity)
            {}
            
            static Properties DEFAULTS_2D(bool useMipmap = true, int slotCapacity = 1024)
            {
                return Properties(useMipmap, false, 2, slotCapacity);
            }
            
            static Properties DEFAULTS_3D(int slotCapacity = 2048)
            {
                return Properties(true, true, 3, slotCapacity);
            }
        };
        
        InputSourceRef inputSource;
        bool useMipmap;
        bool useAnisotropy;
        int maxDimensions;
        int slotCapacity;
        
        XFont(InputSourceRef inputSource, const Properties &properties);
        ~XFont();
        
        void unload();
        void reload();
        
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
        ci::Vec2f getAxis() const;

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
        
        FontMatrix* getMatrix();
        GLushort* getIndices() const;
        
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
    };
}

namespace chr = chronotext;
