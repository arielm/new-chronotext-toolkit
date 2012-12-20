#pragma once

#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/XFontSequence.h"

#include "cinder/gl/gl.h"
#include "cinder/DataSource.h"

#include <string>
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
        float strikethroughOffset;
        float underlineOffset;
        
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
        
        float *tx1;
        float *ty1;
        float *tx2;
        float *ty2;
        
        bool anisotropyAvailable;
        float maxAnisotropy;
        
        GLuint name;
        
        float size;
        float sizeRatio;
        int began;
        FontMatrix matrix;
        
        GLfloat *vertices;
        GLfloat *coords;
        GLshort *indices;
        
        int sequenceSize;
        int sequenceDimensions;
        XFontSequence *sequence;
        
        void read(ci::DataSourceRef source);
        void addAtlasUnit(char *srcData, char *dstData, int xx, int yy, int ww, int hh);
        void init();
        
        void flush(int count);
        void flushIfRequired();
        
    public:
        bool useMipmap;
        bool useAnisotropy;
        int maxDimensions;
        int slotCapacity;
        
        XFont(ci::DataSourceRef source, bool useMipmap = false, bool useAnisotropy = false, int maxDimensions = 3, int slotCapacity = 1024);
        ~XFont();
        
        bool isSpace(wchar_t c);
        bool isValid(wchar_t c);
        int lookup(wchar_t c);
        
        float getSize();
        void setSize(float size);
        
        float getGlyphWidth(int cc);
        float getCharWidth(wchar_t c);
        float getStringWidth(const std::wstring &s);
        float getSubStringWidth(const std::wstring &s, int begin, int end);
        
        float getHeight();
        float getAscent();
        float getDescent();
        float getStrikethroughOffset();
        float getUnderlineOffset();
        
        FontMatrix* getMatrix();
        GLshort* getIndices();
        
        void begin();
        void end();
        
        void beginSequence(XFontSequence *sequence, int dimensions);
        void endSequence();
        
        void addGlyph(int cc, float x, float y);
        void addGlyph(int cc, float x, float y, float z);
        void addTransformedGlyph(int cc, float x, float y);
        void addTransformedGlyph2D(int cc, float x, float y);
    };
}

namespace chr = chronotext;
