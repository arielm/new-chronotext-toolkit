#include "chronotext/font/XFont.h"

using namespace ci;
using namespace std;

namespace chronotext
{
    XFont::XFont(ci::DataSourceRef source, bool useMipmap, bool useAnisotropy, int maxDimensions, int slotCapacity)
    :
    useMipmap(useMipmap),
    useAnisotropy(useAnisotropy),
    maxDimensions(maxDimensions),
    slotCapacity(slotCapacity)
    {
        read(source);
        init();
    }
    
    XFont::~XFont()
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
        
        glDeleteTextures(1, &name);
        
        delete[] vertices;
        delete[] coords;
        delete[] indices;
    }
    
    void XFont::read(DataSourceRef source)
    {
        IStreamRef in = source->createStream();
        
        string version;
        in->readFixedString(&version, 10);
        
        if (version != "XFONT.002")
        {
            throw;
        }
        
        in->readLittle(&glyphCount);
        
        in->readLittle(&nativeFontSize);
        in->readLittle(&height);
        in->readLittle(&ascent);
        in->readLittle(&descent);
        in->readLittle(&spaceWidth);
        in->readLittle(&strikethroughOffset);
        in->readLittle(&underlineOffset);
        
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
        
        char *atlasData = (char*) calloc(atlasWidth * atlasHeight , 1);
        
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
            
            char *data = new char[glyphWidth * glyphHeight];
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
        
        glGenTextures(1, &name);
        glBindTexture(GL_TEXTURE_2D, name);
        
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
        
        free(atlasData);
    }
    
    void XFont::addAtlasUnit(char *srcData, char *dstData, int xx, int yy, int ww, int hh)
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
        setSize(1);
        
        began = 0;
        sequence = NULL;
        
        // ---
        
        vertices = new GLfloat[slotCapacity * maxDimensions * 4];
        coords = new GLfloat[slotCapacity * 2 * 4];
        indices = new GLshort[slotCapacity * 6];
        
        GLshort *tmp = indices;
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
    
    bool XFont::isSpace(wchar_t c)
    {
        return (c == 0x20) || (c == 0xa0);
    }
    
    bool XFont::isValid(wchar_t c)
    {
        return glyphs.count(c);
    }
    
    int XFont::lookup(wchar_t c)
    {
        if (isSpace(c))
        {
            return -2;
        }
        else
        {
            map<wchar_t, int>::iterator it = glyphs.find(c);
            
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
    
    // ---
    
    float XFont::getSize()
    {
        return size;
    }
    
    void XFont::setSize(float size)
    {
        this->size = size;
        sizeRatio = size / nativeFontSize;
    }
    
    float XFont::getGlyphWidth(int cc)
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
    
    float XFont::getCharWidth(wchar_t c)
    {
        return getGlyphWidth(lookup(c));
    }
    
    float XFont::getStringWidth(const wstring &s)
    {
        return getSubStringWidth(s, 0, s.size());
    }
    
    float XFont::getSubStringWidth(const wstring &s, int begin, int end)
    {
        float width = 0;
        
        for (int i = begin; i < end; i++)
        {
            width += getCharWidth(s[i]);
        }
        
        return width;
    }
    
    float XFont::getHeight()
    {
        return height * sizeRatio;
    }
    
    float XFont::getAscent()
    {
        return ascent * sizeRatio;
    }
    
    float XFont::getDescent()
    {
        return descent * sizeRatio;
    }
    
    float XFont::getStrikethroughOffset()
    {
        return strikethroughOffset * sizeRatio;
    }
    
    float XFont::getUnderlineOffset()
    {
        return underlineOffset * sizeRatio;
    }
    
    FontMatrix* XFont::getMatrix()
    {
        return &matrix;
    }
    
    GLshort* XFont::getIndices()
    {
        return indices;
    }
    
    // --- STATE MANAGEMENT ---
    
    void XFont::begin()
    {
        if (began == 0)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, name);
            
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
    
    // --- BUFFERING ---
    
    void XFont::beginSequence(XFontSequence *sequence, int dimensions)
    {
        sequenceSize = 0;
        sequenceDimensions = dimensions;
        sequenceVertices = vertices;
        sequenceCoords = coords;
        
        if (sequence)
        {
            sequence->begin(this, dimensions);
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
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y)
    {
        if (cc >= 0)
        {
            float x1 = x + le[cc] * sizeRatio;
            float x2 = x1 + w[cc] * sizeRatio;
            float y1 = y - te[cc] * sizeRatio;
            float y2 = y1 + h[cc] * sizeRatio;
            
            *sequenceVertices++ = x1;
            *sequenceVertices++ = y1;
            *sequenceVertices++ = x1;
            *sequenceVertices++ = y2;
            *sequenceVertices++ = x2;
            *sequenceVertices++ = y2;
            *sequenceVertices++ = x2;
            *sequenceVertices++ = y1;
            
            *sequenceCoords++ = tx1[cc];
            *sequenceCoords++ = ty1[cc];
            *sequenceCoords++ = tx1[cc];
            *sequenceCoords++ = ty2[cc];
            *sequenceCoords++ = tx2[cc];
            *sequenceCoords++ = ty2[cc];
            *sequenceCoords++ = tx2[cc];
            *sequenceCoords++ = ty1[cc];
            
            incrementSequence();
        }
    }
    
    void XFont::addGlyph(int cc, float x, float y, float z)
    {
        if (cc >= 0)
        {
            float x1 = x + le[cc] * sizeRatio;
            float x2 = x1 + w[cc] * sizeRatio;
            float y1 = y - te[cc] * sizeRatio;
            float y2 = y1 + h[cc] * sizeRatio;
            
            *sequenceVertices++ = x1;
            *sequenceVertices++ = y1;
            *sequenceVertices++ = z;
            *sequenceVertices++ = x1;
            *sequenceVertices++ = y2;
            *sequenceVertices++ = z;
            *sequenceVertices++ = x2;
            *sequenceVertices++ = y2;
            *sequenceVertices++ = z;
            *sequenceVertices++ = x2;
            *sequenceVertices++ = y1;
            *sequenceVertices++ = z;
            
            *sequenceCoords++ = tx1[cc];
            *sequenceCoords++ = ty1[cc];
            *sequenceCoords++ = tx1[cc];
            *sequenceCoords++ = ty2[cc];
            *sequenceCoords++ = tx2[cc];
            *sequenceCoords++ = ty2[cc];
            *sequenceCoords++ = tx2[cc];
            *sequenceCoords++ = ty1[cc];
            
            incrementSequence();
        }
    }
    
    void XFont::addTransformedGlyph3D(int cc, float x, float y)
    {
        float x1 = x + le[cc] * sizeRatio;
        float x2 = x1 + w[cc] * sizeRatio;
        float y1 = y - te[cc] * sizeRatio;
        float y2 = y1 + h[cc] * sizeRatio;
        
        matrix.transform3D(x1, y2, x2, y1, sequenceVertices);
        sequenceVertices += 4 * 3;
        
        *sequenceCoords++ = tx1[cc];
        *sequenceCoords++ = ty1[cc];
        *sequenceCoords++ = tx1[cc];
        *sequenceCoords++ = ty2[cc];
        *sequenceCoords++ = tx2[cc];
        *sequenceCoords++ = ty2[cc];
        *sequenceCoords++ = tx2[cc];
        *sequenceCoords++ = ty1[cc];
        
        incrementSequence();
    }
    
    void XFont::addTransformedGlyph2D(int cc, float x, float y)
    {
        float x1 = x + le[cc] * sizeRatio;
        float x2 = x1 + w[cc] * sizeRatio;
        float y1 = y - te[cc] * sizeRatio;
        float y2 = y1 + h[cc] * sizeRatio;
        
        matrix.transform2D(x1, y2, x2, y1, sequenceVertices);
        sequenceVertices += 4 * 2;
        
        *sequenceCoords++ = tx1[cc];
        *sequenceCoords++ = ty1[cc];
        *sequenceCoords++ = tx1[cc];
        *sequenceCoords++ = ty2[cc];
        *sequenceCoords++ = tx2[cc];
        *sequenceCoords++ = ty2[cc];
        *sequenceCoords++ = tx2[cc];
        *sequenceCoords++ = ty1[cc];
        
        incrementSequence();
    }
}
