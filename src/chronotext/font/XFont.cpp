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
    /*
     * XXX: COULD NOT GET C++11'S DELEGATED CONSTRUCTOR TO COMPILE WITH VISUAL STUDIO 2012
     */
    XFont::XFont(const string &resourceName, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    :
    inputSource(InputSource::getResource(resourceName)),
    useMipmap(useMipmap),
    useAnisotropy(useAnisotropy),
    maxDimensions(maxDimensions),
    slotCapacity(slotCapacity),
    unloaded(true)
    {
        reload();
        
        setSize(1);
        setDirection(+1);
        setAxis(Vec2f(+1, +1));
    }
    
    XFont::XFont(InputSourceRef source, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    :
    inputSource(source),
    useMipmap(useMipmap),
    useAnisotropy(useAnisotropy),
    maxDimensions(maxDimensions),
    slotCapacity(slotCapacity),
    unloaded(true)
    {
        reload();
        
        setSize(1);
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
            delete[] w;
            delete[] h;
            delete[] le;
            delete[] te;
            delete[] lw;
            
            delete[] tx1;
            delete[] ty1;
            delete[] tx2;
            delete[] ty2;
            
            delete[] indices;
            delete[] vertices;
            delete[] coords;
            
            glDeleteTextures(1, &textureName);
            
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
        in->readLittle(&spaceWidth);
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
        lw = new float[glyphCount];
        
        tx1 = new float[glyphCount];
        ty1 = new float[glyphCount];
        tx2 = new float[glyphCount];
        ty2 = new float[glyphCount];
        
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
            
            in->readLittle(&lw[i]);
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
            
            tx1[i] = xx / ww;
            ty1[i] = yy / hh;
            tx2[i] = (xx + w[i]) / ww;
            ty2[i] = (yy + h[i]) / hh;
        }
        
        // ---
        
        anisotropyAvailable = strstr((char*)glGetString(GL_EXTENSIONS), "GL_EXT_texture_filter_anisotropic");
        
        if (anisotropyAvailable)
        {
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        }
        
        // ---
        
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
    
    void XFont::init()
    {
        matrix.setToIdentity();
        began = 0;
        sequence = NULL;
        
        // ---
        
        indices = new GLushort[slotCapacity * 6];
        vertices = new float[slotCapacity * maxDimensions * 4];
        coords = new float[slotCapacity * 2 * 4];
        
        /*
         * FILLING THE INDICES WITH A QUAD PATTERN
         */
        
        GLushort *tmp = indices;
        int offset = 0;
        
        for (int i = 0; i < slotCapacity; i++)
        {
            *tmp++ = offset;
            *tmp++ = offset + 1;
            *tmp++ = offset + 2;
            *tmp++ = offset + 2;
            *tmp++ = offset + 3;
            *tmp++ = offset;
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
    
    int XFont::lookup(wchar_t c) const
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
        
        for (auto entry : glyphs)
        {
            characters.push_back(entry.first);
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
    
    float XFont::getGlyphWidth(int cc) const
    {
        if (cc == - 2)
        {
            return spaceWidth * sizeRatio;
        }
        else if (cc == - 1)
        {
            return 0;
        }
        else
        {
            return lw[cc] * sizeRatio;
        }
    }
    
    float XFont::getCharWidth(wchar_t c) const
    {
        return getGlyphWidth(lookup(c));
    }
    
    float XFont::getStringWidth(const wstring &s) const
    {
        return getSubStringWidth(s, 0, s.size());
    }
    
    float XFont::getSubStringWidth(const wstring &s, int begin, int end) const
    {
        float width = 0;
        
        for (auto c : s)
        {
            width += getCharWidth(c);
        }
        
        return width;
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
        return indices;
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
        sequenceCoords = coords;
        
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
            sequence->flush(vertices, coords, sequenceSize);
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
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(sequenceDimensions, GL_FLOAT, 0, vertices);
        glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, indices);
    }
    
    void XFont::incrementSequence()
    {
        sequenceSize++;
        
        if (sequenceSize == slotCapacity)
        {
            if (sequence)
            {
                sequence->flush(vertices, coords, sequenceSize);
            }
            else
            {
                flush(sequenceSize);
            }
            
            sequenceSize = 0;
            sequenceVertices = vertices;
            sequenceCoords = coords;
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
            quad.tx1 = tx1[cc];
            quad.tx2 = tx2[cc];
        }
        else
        {
            quad.tx1 = tx2[cc];
            quad.tx2 = tx1[cc];
        }
        
        if (axis.y > 0)
        {
            quad.y1 = y - te[cc] * sizeRatio;
            quad.y2 = quad.y1 + h[cc] * sizeRatio;
            
            quad.ty1 = ty1[cc];
            quad.ty2 = ty2[cc];
        }
        else
        {
            quad.y2 = y + te[cc] * sizeRatio;
            quad.y1 = quad.y2 - h[cc] * sizeRatio;
            
            quad.ty1 = ty2[cc];
            quad.ty2 = ty1[cc];
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
                quad.tx1 += axis.x * dx / atlasWidth / sizeRatio;;
            }
            
            if (quad.x2 > clip.x2)
            {
                float dx = clip.x2 - quad.x2;
                quad.x2 += dx;
                quad.tx2 += axis.x * dx / atlasWidth / sizeRatio;;
            }
            
            if (quad.y1 < clip.y1)
            {
                float dy = clip.y1 - quad.y1;
                quad.y1 += dy;
                quad.ty1 += axis.y * dy / atlasHeight / sizeRatio;
            }
            
            if (quad.y2 > clip.y2)
            {
                float dy = clip.y2 - quad.y2;
                quad.y2 += dy;
                quad.ty2 += axis.y * dy / atlasHeight / sizeRatio;
            }
            
            return true;
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            const GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            *sequenceVertices++ = quad.x1;
            *sequenceVertices++ = quad.y1;
            *sequenceVertices++ = quad.x1;
            *sequenceVertices++ = quad.y2;
            *sequenceVertices++ = quad.x2;
            *sequenceVertices++ = quad.y2;
            *sequenceVertices++ = quad.x2;
            *sequenceVertices++ = quad.y1;
            
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty1;
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty1;
            
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
                *sequenceVertices++ = quad.x1;
                *sequenceVertices++ = quad.y1;
                *sequenceVertices++ = quad.x1;
                *sequenceVertices++ = quad.y2;
                *sequenceVertices++ = quad.x2;
                *sequenceVertices++ = quad.y2;
                *sequenceVertices++ = quad.x2;
                *sequenceVertices++ = quad.y1;
                
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty1;
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty1;
                
                incrementSequence();
            }
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y, float z)
    {
        if (cc >= 0)
        {
            const GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            *sequenceVertices++ = quad.x1;
            *sequenceVertices++ = quad.y1;
            *sequenceVertices++ = z;
            *sequenceVertices++ = quad.x1;
            *sequenceVertices++ = quad.y2;
            *sequenceVertices++ = z;
            *sequenceVertices++ = quad.x2;
            *sequenceVertices++ = quad.y2;
            *sequenceVertices++ = z;
            *sequenceVertices++ = quad.x2;
            *sequenceVertices++ = quad.y1;
            *sequenceVertices++ = z;
            
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty1;
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty1;
            
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
                *sequenceVertices++ = quad.x1;
                *sequenceVertices++ = quad.y1;
                *sequenceVertices++ = z;
                *sequenceVertices++ = quad.x1;
                *sequenceVertices++ = quad.y2;
                *sequenceVertices++ = z;
                *sequenceVertices++ = quad.x2;
                *sequenceVertices++ = quad.y2;
                *sequenceVertices++ = z;
                *sequenceVertices++ = quad.x2;
                *sequenceVertices++ = quad.y1;
                *sequenceVertices++ = z;
                
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty1;
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty1;
                
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph2D(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            const GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            sequenceVertices += matrix.transform2D(quad.x1, quad.y2, quad.x2, quad.y1, sequenceVertices);
            
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty1;
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty1;
            
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
                sequenceVertices += matrix.transform2D(quad.x1, quad.y2, quad.x2, quad.y1, sequenceVertices);
                
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty1;
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty1;
                
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph3D(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            const GlyphQuad quad = getGlyphQuad(cc, x, y);
            
            sequenceVertices += matrix.transform3D(quad.x1, quad.y2, quad.x2, quad.y1, sequenceVertices);
            
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty1;
            *sequenceCoords++ = quad.tx1;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty2;
            *sequenceCoords++ = quad.tx2;
            *sequenceCoords++ = quad.ty1;
            
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
                sequenceVertices += matrix.transform3D(quad.x1, quad.y2, quad.x2, quad.y1, sequenceVertices);
                
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty1;
                *sequenceCoords++ = quad.tx1;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty2;
                *sequenceCoords++ = quad.tx2;
                *sequenceCoords++ = quad.ty1;
                
                incrementSequence();
            }
        }
    }
}
