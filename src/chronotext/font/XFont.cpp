/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/XFont.h"
#include "chronotext/utils/Utils.h"

using namespace ci;
using namespace std;

namespace chronotext
{
    XFont::XFont(InputSourceRef source, const Properties &properties)
    :
    inputSource(source),
    useMipmap(properties.useMipmap),
    useAnisotropy(properties.useAnisotropy),
    maxDimensions(properties.maxDimensions),
    slotCapacity(properties.slotCapacity),
    unloaded(true)
    {
        reload();
        
        // ---
        
        anisotropyAvailable = gl::isExtensionAvailable("GL_EXT_texture_filter_anisotropic");
        
        if (anisotropyAvailable)
        {
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        }
        
        // ---
        
        setSize(nativeFontSize);
        setDirection(+1);
        setAxis(Vec2f(+1, +1));
    }
    
    XFont::~XFont()
    {
        unload();
    }
    
    void XFont::unload()
    {
        if (!unloaded)
        {
            glDeleteTextures(1, &textureName);

            delete[] w;
            delete[] h;
            delete[] le;
            delete[] te;
            delete[] advance;
            
            delete[] u1;
            delete[] v1;
            delete[] u2;
            delete[] v2;

            delete[] vertices;
            
            // ---
            
            unloaded = true;
            
            LOGD <<
            "FONT UNLOADED: " <<
            textureName <<
            endl;
        }
    }
    
    void XFont::reload()
    {
        if (unloaded)
        {
            read(inputSource->loadDataSource()->createStream());
            init();
            
            unloaded = false;
            
            LOGD <<
            "FONT UPLOADED: " <<
            inputSource->getFilePathHint() << " | " <<
            textureName << " | " <<
            atlasWidth << "x" << atlasHeight  <<
            endl;
        }
    }
    
    void XFont::read(IStreamRef in)
    {
        string version;
        in->readFixedString(&version, 10);
        
        if (version != "XFONT.003")
        {
            throw runtime_error("XFont: WRONG FORMAT");
        }
        
        in->readLittle(&glyphCount);
        
        in->readLittle(&nativeFontSize);
        in->readLittle(&height);
        in->readLittle(&ascent);
        in->readLittle(&descent);
        in->readLittle(&spaceAdvance);
        in->readLittle(&strikethroughFactor);
        in->readLittle(&underlineOffset);
        in->readLittle(&lineThickness);
        
        in->readLittle(&atlasWidth);
        in->readLittle(&atlasHeight);
        in->readLittle(&atlasPadding);
        in->readLittle(&unitMargin);
        in->readLittle(&unitPadding);
        
        // ---
        
        w = new float[glyphCount];
        h = new float[glyphCount];
        le = new float[glyphCount];
        te = new float[glyphCount];
        advance = new float[glyphCount];
        
        u1 = new float[glyphCount];
        v1 = new float[glyphCount];
        u2 = new float[glyphCount];
        v2 = new float[glyphCount];
        
        auto atlasData = new unsigned char[atlasWidth * atlasHeight](); // ZERO-FILLED
        
        for (int i = 0; i < glyphCount; i++)
        {
            int glyphChar;
            int glyphWidth;
            int glyphHeight;
            int glyphLeftExtent;
            int glyphTopExtent;
            int glyphAtlasX;
            int glyphAtlasY;
            
            in->readLittle(&glyphChar);
            glyphs[(wchar_t)glyphChar] = i;
            
            in->readLittle(&advance[i]);
            in->readLittle(&glyphWidth);
            in->readLittle(&glyphHeight);
            in->readLittle(&glyphLeftExtent);
            in->readLittle(&glyphTopExtent);
            in->readLittle(&glyphAtlasX);
            in->readLittle(&glyphAtlasY);
            
            w[i] = glyphWidth + unitPadding * 2;
            h[i] = glyphHeight + unitPadding * 2;
            le[i] = glyphLeftExtent - unitPadding;
            te[i] = glyphTopExtent + unitPadding;
            
            unsigned char *data = new unsigned char[glyphWidth * glyphHeight];
            in->readData(data, glyphWidth * glyphHeight);
            addAtlasUnit(data, atlasData, glyphAtlasX + atlasPadding + unitMargin, glyphAtlasY + atlasPadding + unitMargin, glyphWidth, glyphHeight);
            delete[] data;
            
            int xx = glyphAtlasX + atlasPadding + unitMargin - unitPadding;
            int yy = glyphAtlasY + atlasPadding + unitMargin - unitPadding;
            
            float ww = (float)atlasWidth;
            float hh = (float)atlasHeight;
            
            u1[i] = xx / ww;
            v1[i] = yy / hh;
            u2[i] = (xx + w[i]) / ww;
            v2[i] = (yy + h[i]) / hh;
        }

        uploadAtlasData(atlasData);
        delete[] atlasData;
    }
    
    void XFont::addAtlasUnit(unsigned char *srcData, unsigned char *dstData, int xx, int yy, int ww, int hh)
    {
        for (int iy = 0; iy < hh; iy++)
        {
            for (int ix = 0; ix < ww; ix++)
            {
                dstData[(iy + yy) * atlasWidth + ix + xx] = srcData[iy * ww + ix];
            }
        }
    }
    
    void XFont::uploadAtlasData(unsigned char *atlasData)
    {
        glGenTextures(1, &textureName);
        glBindTexture(GL_TEXTURE_2D, textureName);
        
        if (useMipmap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        if (useMipmap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlasWidth, atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlasData);
        
        if (useMipmap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void XFont::init()
    {
        matrix.setToIdentity();
        began = 0;
        sequence = NULL;
        
        // ---
        
        vertices = new float[slotCapacity * (maxDimensions * 4 + 2 * 4)];
        
        /*
         * FILLING THE INDICES WITH A QUAD PATTERN
         */

        indices.reserve(slotCapacity * 6);
        int offset = 0;
        
        for (int i = 0; i < slotCapacity; i++)
        {
            indices.push_back(offset);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset);
            offset += 4;
        }
    }
    
    // ---
    
    bool XFont::isSpace(wchar_t c) const
    {
        return (c == 0x20) || (c == 0xa0);
    }
    
    bool XFont::isValid(wchar_t c) const
    {
        return glyphs.count(c);
    }
    
    int XFont::getGlyphIndex(wchar_t c) const
    {
        if (isSpace(c))
        {
            return -2;
        }
        else
        {
            auto it = glyphs.find(c);
            
            if (it == glyphs.end())
            {
                return -1; // TODO: RETURN "MISSING GLYPH"
            }
            else
            {
                return it->second;
            }
        }
    }
    
    wstring XFont::getCharacters() const
    {
        wstring characters;
        characters.reserve(glyphs.size());
        
        for (auto it : glyphs)
        {
            characters.push_back(it.first);
        }
        
        return characters;
    }
    
    // ---
    
    void XFont::setSize(float size)
    {
        this->size = size;
        sizeRatio = size / nativeFontSize;
    }
    
    void XFont::setDirection(float direction)
    {
        this->direction = direction;
    }
    
    void XFont::setAxis(const Vec2f &axis)
    {
        this->axis = axis;
    }
    
    void XFont::setStrikethroughFactor(float factor)
    {
        strikethroughFactor = factor;
    }
    
    float XFont::getSize() const
    {
        return size;
    }
    
    float XFont::getDirection() const
    {
        return direction * axis.x;
    }
    
    Vec2f XFont::getAxis() const
    {
        return axis;
    }
    
    float XFont::getGlyphAdvance(int cc) const
    {
        if (cc == -2)
        {
            return spaceAdvance * sizeRatio;
        }
        else if (cc == -1)
        {
            return 0;
        }
        else
        {
            return advance[cc] * sizeRatio;
        }
    }
    
    float XFont::getCharAdvance(wchar_t c) const
    {
        return getGlyphAdvance(getGlyphIndex(c));
    }
    
    float XFont::getStringAdvance(const wstring &s) const
    {
        return getSubStringAdvance(s, 0, s.size());
    }
    
    float XFont::getSubStringAdvance(const wstring &s, int begin, int end) const
    {
        float advance = 0;
        
        for (int i = begin; i < end; i++)
        {
            advance += getCharAdvance(s.at(i));
        }
        
        return advance;
    }
    
    float XFont::getHeight() const
    {
        return height * sizeRatio;
    }
    
    float XFont::getAscent() const
    {
        return ascent * sizeRatio;
    }
    
    float XFont::getDescent() const
    {
        return descent * sizeRatio;
    }
    
    float XFont::getStrikethroughOffset() const
    {
        return strikethroughFactor * (ascent - descent) * sizeRatio;
    }
    
    float XFont::getUnderlineOffset() const
    {
        return underlineOffset * sizeRatio;
    }
    
    float XFont::getLineThickness() const
    {
        return lineThickness * sizeRatio;
    }
    
    FontMatrix* XFont::getMatrix()
    {
        return &matrix;
    }
    
    GLushort* XFont::getIndices() const
    {
        return const_cast<GLushort*>(indices.data());
    }
    
    void XFont::begin()
    {
        if (began == 0)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textureName);
            
            if (useAnisotropy && anisotropyAvailable)
            {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
            }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        
        began++;
    }
    
    void XFont::end()
    {
        began--;
        
        if (began == 0)
        {
            if (useAnisotropy && anisotropyAvailable)
            {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
            }
            
            glDisable(GL_TEXTURE_2D);
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    
    void XFont::beginSequence(XFontSequence *sequence, int dimensions)
    {
        sequenceDimensions = dimensions;

        sequenceSize = 0;
        sequenceVertices = vertices;
        
        if (sequence)
        {
            sequence->begin(this, dimensions, slotCapacity);
            this->sequence = sequence;
        }
        else
        {
            begin();
        }
    }
    
    void XFont::endSequence()
    {
        if (sequence)
        {
            sequence->flush(vertices, sequenceSize);
            sequence->end();
            sequence = NULL;
        }
        else
        {
            flush(sequenceSize);
            end();
        }
    }
    
    void XFont::flush(int count)
    {
        int stride = sizeof(float) * (sequenceDimensions + 2);

        glVertexPointer(sequenceDimensions, GL_FLOAT, stride, vertices);
        glTexCoordPointer(2, GL_FLOAT, stride, vertices + sequenceDimensions);
        glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, indices.data());
    }
    
    void XFont::incrementSequence()
    {
        sequenceSize++;
        
        if (sequenceSize == slotCapacity)
        {
            if (sequence)
            {
                sequence->flush(vertices, sequenceSize);
            }
            else
            {
                flush(sequenceSize);
            }
            
            sequenceSize = 0;
            sequenceVertices = vertices;
        }
    }
    
    GlyphQuad XFont::getGlyphQuad(int cc, float x, float y) const
    {
        GlyphQuad quad;
        
        if (direction * axis.x > 0)
        {
            quad.x1 = x + le[cc] * sizeRatio;
            quad.x2 = quad.x1 + w[cc] * sizeRatio;
        }
        else
        {
            quad.x2 = x - le[cc] * sizeRatio;
            quad.x1 = quad.x2 - w[cc] * sizeRatio;
        }
        
        if (axis.x > 0)
        {
            quad.u1 = u1[cc];
            quad.u2 = u2[cc];
        }
        else
        {
            quad.u1 = u2[cc];
            quad.u2 = u1[cc];
        }
        
        if (axis.y > 0)
        {
            quad.y1 = y - te[cc] * sizeRatio;
            quad.y2 = quad.y1 + h[cc] * sizeRatio;
            
            quad.v1 = v1[cc];
            quad.v2 = v2[cc];
        }
        else
        {
            quad.y2 = y + te[cc] * sizeRatio;
            quad.y1 = quad.y2 - h[cc] * sizeRatio;
            
            quad.v1 = v2[cc];
            quad.v2 = v1[cc];
        }
        
        return quad;
    }
    
    bool XFont::computeClip(GlyphQuad &quad, const ci::Rectf &clip)
    {
        if ((quad.x1 > clip.x2 ) || (quad.x2 < clip.x1) || (quad.y1 > clip.y2) || (quad.y2 < clip.y1))
        {
            return false;
        }
        else
        {
            if (quad.x1 < clip.x1)
            {
                float dx = clip.x1 - quad.x1;
                quad.x1 += dx;
                quad.u1 += axis.x * dx / atlasWidth / sizeRatio;;
            }
            
            if (quad.x2 > clip.x2)
            {
                float dx = clip.x2 - quad.x2;
                quad.x2 += dx;
                quad.u2 += axis.x * dx / atlasWidth / sizeRatio;;
            }
            
            if (quad.y1 < clip.y1)
            {
                float dy = clip.y1 - quad.y1;
                quad.y1 += dy;
                quad.v1 += axis.y * dy / atlasHeight / sizeRatio;
            }
            
            if (quad.y2 > clip.y2)
            {
                float dy = clip.y2 - quad.y2;
                quad.y2 += dy;
                quad.v2 += axis.y * dy / atlasHeight / sizeRatio;
            }
            
            return true;
        }
    }
    
    int XFont::addGlyph(const GlyphQuad &quad, float *vertices)
    {
        *vertices++ = quad.x1;
        *vertices++ = quad.y1;
        
        *vertices++ = quad.u1;
        *vertices++ = quad.v1;
        
        //
        
        *vertices++ = quad.x1;
        *vertices++ = quad.y2;
        
        *vertices++ = quad.u1;
        *vertices++ = quad.v2;
        
        //
        
        *vertices++ = quad.x2;
        *vertices++ = quad.y2;
        
        *vertices++ = quad.u2;
        *vertices++ = quad.v2;
        
        //
        
        *vertices++ = quad.x2;
        *vertices++ = quad.y1;
        
        *vertices++ = quad.u2;
        *vertices++ = quad.v1;
        
        return 4 * (2 + 2);
    }
    
    int XFont::addGlyph(const GlyphQuad &quad, float z, float *vertices)
    {
        *vertices++ = quad.x1;
        *vertices++ = quad.y1;
        *vertices++ = z;
        
        *vertices++ = quad.u1;
        *vertices++ = quad.v1;
        
        //
        
        *vertices++ = quad.x1;
        *vertices++ = quad.y2;
        *vertices++ = z;
        
        *vertices++ = quad.u1;
        *vertices++ = quad.v2;
        
        //
        
        *vertices++ = quad.x2;
        *vertices++ = quad.y2;
        *vertices++ = z;
        
        *vertices++ = quad.u2;
        *vertices++ = quad.v2;
        
        //
        
        *vertices++ = quad.x2;
        *vertices++ = quad.y1;
        *vertices++ = z;
        
        *vertices++ = quad.u2;
        *vertices++ = quad.v1;
        
        return 4 * (3 + 2);
    }

    
    void XFont::addGlyph(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            sequenceVertices += addGlyph(getGlyphQuad(cc, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y, const Rectf &clip)
    {
        if (cc >= 0)
        {
            GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += addGlyph(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y, float z)
    {
        if (cc >= 0)
        {
            sequenceVertices += addGlyph(getGlyphQuad(cc, x, y), z, sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y, float z, const Rectf &clip)
    {
        if (cc >= 0)
        {
            GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += addGlyph(quad, z, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph2D(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            sequenceVertices += matrix.addTransformedQuad2D(getGlyphQuad(cc, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addTransformedGlyph2D(int cc, float x, float y, const Rectf &clip)
    {
        if (cc >= 0)
        {
            GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += matrix.addTransformedQuad2D(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph3D(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            sequenceVertices += matrix.addTransformedQuad3D(getGlyphQuad(cc, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addTransformedGlyph3D(int cc, float x, float y, const Rectf &clip)
    {
        if (cc >= 0)
        {
            GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += matrix.addTransformedQuad3D(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
}
