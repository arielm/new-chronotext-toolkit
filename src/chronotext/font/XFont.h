#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/font/FontMatrix.h"
#include "chronotext/font/XFontSequence.h"

#include <string>
#include <map>

namespace chronotext
{
    struct GlyphQuad
    {
        float x1;
        float y1;
        float x2;
        float y2;
        
        float tx1;
        float ty1;
        float tx2;
        float ty2;
    };
    
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
        float direction;
        ci::Vec2f axis;
        
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
        
        bool unloaded;
        
        void read(ci::IStreamRef in);
        void addAtlasUnit(char *srcData, char *dstData, int xx, int yy, int ww, int hh);
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
        
        bool isSpace(wchar_t c);
        bool isValid(wchar_t c);
        int lookup(wchar_t c);
        
        void setSize(float size);
        void setDirection(float direction);
        void setAxis(const ci::Vec2f &axis);

        float getSize();
        float getDirection();
        ci::Vec2f getAxis() const;

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
