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
        float nativeFontSize;
        float height;
        float ascent;
        float descent;
        float spaceWidth;
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
        float *lw;
        
        float *u1;
        float *v1;
        float *u2;
        float *v2;
        
        bool anisotropyAvailable;
        float maxAnisotropy;

        float size;
        float sizeRatio;
        float direction;
        ci::Vec2f axis;
        
        int began;
        FontMatrix matrix;

        GLuint textureName;

        GLushort *indices;
        float *vertices;
        
        int sequenceSize;
        int sequenceDimensions;
        float *sequenceVertices;
        XFontSequence *sequence;
        
        bool unloaded;
        
        void read(ci::IStreamRef in);
        void addAtlasUnit(unsigned char *srcData, unsigned char *dstData, int xx, int yy, int ww, int hh);
        void init();
        
        void flush(int count);
        void incrementSequence();
        
        GlyphQuad getGlyphQuad(int cc, float x, float y) const;
        bool computeClip(GlyphQuad &quad, const ci::Rectf &clip);
        
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
        
        InputSourceRef inputSource;
        bool useMipmap;
        bool useAnisotropy;
        int maxDimensions;
        int slotCapacity;
        
        XFont(const std::string &resourceName, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
        XFont(InputSourceRef inputSource, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
        
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

        float getGlyphWidth(int cc) const;
        float getCharWidth(wchar_t c) const;
        float getStringWidth(const std::wstring &s) const;
        float getSubStringWidth(const std::wstring &s, int begin, int end) const;
        
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
        
        void addGlyph(int cc, float x, float y);
        void addGlyph(int cc, float x, float y, const ci::Rectf &clip);

        void addGlyph(int cc, float x, float y, float z);
        void addGlyph(int cc, float x, float y, float z, const ci::Rectf &clip);
        
        void addTransformedGlyph2D(int cc, float x, float y);
        void addTransformedGlyph2D(int cc, float x, float y, const ci::Rectf &clip);
        
        void addTransformedGlyph3D(int cc, float x, float y);
        void addTransformedGlyph3D(int cc, float x, float y, const ci::Rectf &clip);
    };
}

namespace chr = chronotext;
