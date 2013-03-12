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

        float size;
        float sizeRatio;
        int began;
        FontMatrix matrix;

        GLuint textureName;

        GLushort *indices;
        GLfloat *vertices;
        GLfloat *coords;
        
        int sequenceSize;
        int sequenceDimensions;
        GLfloat *sequenceVertices;
        GLfloat *sequenceCoords;
        XFontSequence *sequence;
        
        void read(ci::IStreamRef in);
        void addAtlasUnit(char *srcData, char *dstData, int xx, int yy, int ww, int hh);
        void init();
        
        void flush(int count);
        void incrementSequence();
        bool computeClip(float *x1, float *y1, float *x2, float *y2, float *tx1, float *ty1, float *tx2, float *ty2, const ci::Rectf &clip);
        
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
        GLushort* getIndices();
        
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
