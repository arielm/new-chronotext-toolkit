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
    unloaded(true),
    began(0),
    sequence(NULL)
    {
        anisotropyAvailable = gl::isExtensionAvailable("GL_EXT_texture_filter_anisotropic");
        
        if (anisotropyAvailable)
        {
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        }
        
        // ---
        
        reload();
        
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
        
        // ---
        
        setSize(nativeFontSize);
        setDirection(+1);
        setAxis(Vec2f(+1, +1));
    }
    
    XFont::~XFont()
    {
        delete[] vertices;
        unload();
    }
    
    void XFont::unload()
    {
        if (!unloaded)
        {
            glDeleteTextures(1, &textureName);
            data.reset();
            
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
            auto tmp = fetchFont(inputSource);
            data = unique_ptr<FontData>(tmp.first);
            auto atlas = unique_ptr<FontAtlas>(tmp.second);

            glyphCount = data->glyphCount;
            glyphs = data->glyphs;

            nativeFontSize = data->nativeFontSize;
            height = data->height;
            ascent = data->ascent;
            descent = data->descent;
            spaceAdvance = data->spaceAdvance;
            strikethroughFactor = data->strikethroughFactor;
            underlineOffset = data->underlineOffset;
            lineThickness = data->lineThickness;
            
            w = data->w;
            h = data->h;
            le = data->le;
            te = data->te;
            advance = data->advance;
            
            u1 = data->u1;
            v1 = data->v1;
            u2 = data->u2;
            v2 = data->v2;

            textureWidth = atlas->width;
            textureHeight = atlas->height;
            textureName = uploadAtlasData(atlas.get(), useMipmap);
            
            // ---
            
            unloaded = false;
            
            LOGD <<
            "FONT UPLOADED: " <<
            inputSource->getFilePathHint() << " | " <<
            textureName << " | " <<
            textureWidth << "x" << textureHeight  <<
            endl;
        }
    }
    
    std::pair<FontData*, FontAtlas*> XFont::fetchFont(InputSourceRef source)
    {
        auto in = source->loadDataSource()->createStream(); // CAN THROW
        
        string version;
        in->readFixedString(&version, 10);
        
        if (version != "XFONT.003")
        {
            throw runtime_error("XFont: WRONG FORMAT");
        }
        
        // ---
        
        int glyphCount;
        in->readLittle(&glyphCount);
        
        auto data = new FontData(glyphCount);
        
        in->readLittle(&data->nativeFontSize);
        in->readLittle(&data->height);
        in->readLittle(&data->ascent);
        in->readLittle(&data->descent);
        in->readLittle(&data->spaceAdvance);
        in->readLittle(&data->strikethroughFactor);
        in->readLittle(&data->underlineOffset);
        in->readLittle(&data->lineThickness);
        
        int atlasWidth;
        int atlasHeight;
        int atlasPadding;
        int unitMargin;
        int unitPadding;
        
        in->readLittle(&atlasWidth);
        in->readLittle(&atlasHeight);
        in->readLittle(&atlasPadding);
        in->readLittle(&unitMargin);
        in->readLittle(&unitPadding);
        
        auto atlas = new FontAtlas(atlasWidth, atlasHeight);

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
            data->glyphs[(wchar_t)glyphChar] = i;
            
            in->readLittle(&data->advance[i]);
            in->readLittle(&glyphWidth);
            in->readLittle(&glyphHeight);
            in->readLittle(&glyphLeftExtent);
            in->readLittle(&glyphTopExtent);
            in->readLittle(&glyphAtlasX);
            in->readLittle(&glyphAtlasY);

            auto glyphData = new unsigned char[glyphWidth * glyphHeight];
            in->readData(glyphData, glyphWidth * glyphHeight);
            addAtlasUnit(atlas, glyphData, glyphAtlasX + atlasPadding + unitMargin, glyphAtlasY + atlasPadding + unitMargin, glyphWidth, glyphHeight);
            delete[] glyphData;
            
            data->w[i] = glyphWidth + unitPadding * 2;
            data->h[i] = glyphHeight + unitPadding * 2;
            data->le[i] = glyphLeftExtent - unitPadding;
            data->te[i] = glyphTopExtent + unitPadding;
            
            int x = glyphAtlasX + atlasPadding + unitMargin - unitPadding;
            int y = glyphAtlasY + atlasPadding + unitMargin - unitPadding;
            
            data->u1[i] = x / (float)atlasWidth;
            data->v1[i] = y / (float)atlasHeight;
            data->u2[i] = (x + data->w[i]) / (float)atlasWidth;
            data->v2[i] = (y + data->h[i]) / (float)atlasHeight;
        }
        
        return make_pair(data, atlas);
    }
    
    void XFont::addAtlasUnit(FontAtlas *atlas, unsigned char *glyphData, int x, int y, int width, int height)
    {
        for (int iy = 0; iy < height; iy++)
        {
            for (int ix = 0; ix < width; ix++)
            {
                atlas->data[(iy + y) * atlas->width + ix + x] = glyphData[iy * width + ix];
            }
        }
    }
    
    GLuint XFont::uploadAtlasData(FontAtlas *atlas, bool useMipmap)
    {
        GLuint name;
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
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, atlas->width, atlas->height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, atlas->data);
        
        if (useMipmap)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return name;
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
    
    float XFont::getGlyphAdvance(int glyphIndex) const
    {
        if (glyphIndex == -2)
        {
            return spaceAdvance * sizeRatio;
        }
        else if (glyphIndex == -1)
        {
            return 0;
        }
        else
        {
            return advance[glyphIndex] * sizeRatio;
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
    
    GlyphQuad XFont::getGlyphQuad(int glyphIndex, float x, float y) const
    {
        GlyphQuad quad;
        
        if (direction * axis.x > 0)
        {
            quad.x1 = x + le[glyphIndex] * sizeRatio;
            quad.x2 = quad.x1 + w[glyphIndex] * sizeRatio;
        }
        else
        {
            quad.x2 = x - le[glyphIndex] * sizeRatio;
            quad.x1 = quad.x2 - w[glyphIndex] * sizeRatio;
        }
        
        if (axis.x > 0)
        {
            quad.u1 = u1[glyphIndex];
            quad.u2 = u2[glyphIndex];
        }
        else
        {
            quad.u1 = u2[glyphIndex];
            quad.u2 = u1[glyphIndex];
        }
        
        if (axis.y > 0)
        {
            quad.y1 = y - te[glyphIndex] * sizeRatio;
            quad.y2 = quad.y1 + h[glyphIndex] * sizeRatio;
            
            quad.v1 = v1[glyphIndex];
            quad.v2 = v2[glyphIndex];
        }
        else
        {
            quad.y2 = y + te[glyphIndex] * sizeRatio;
            quad.y1 = quad.y2 - h[glyphIndex] * sizeRatio;
            
            quad.v1 = v2[glyphIndex];
            quad.v2 = v1[glyphIndex];
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
                quad.u1 += axis.x * dx / textureWidth / sizeRatio;;
            }
            
            if (quad.x2 > clip.x2)
            {
                float dx = clip.x2 - quad.x2;
                quad.x2 += dx;
                quad.u2 += axis.x * dx / textureWidth / sizeRatio;;
            }
            
            if (quad.y1 < clip.y1)
            {
                float dy = clip.y1 - quad.y1;
                quad.y1 += dy;
                quad.v1 += axis.y * dy / textureHeight / sizeRatio;
            }
            
            if (quad.y2 > clip.y2)
            {
                float dy = clip.y2 - quad.y2;
                quad.y2 += dy;
                quad.v2 += axis.y * dy / textureHeight / sizeRatio;
            }
            
            return true;
        }
    }
    
    int XFont::addQuad(const GlyphQuad &quad, float *vertices)
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
    
    int XFont::addQuad(const GlyphQuad &quad, float z, float *vertices)
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

    
    void XFont::addGlyph(int glyphIndex, float x, float y)
    {
        if (glyphIndex >= 0)
        {
            sequenceVertices += addQuad(getGlyphQuad(glyphIndex, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addGlyph(int glyphIndex, float x, float y, const Rectf &clip)
    {
        if (glyphIndex >= 0)
        {
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += addQuad(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addGlyph(int glyphIndex, float x, float y, float z)
    {
        if (glyphIndex >= 0)
        {
            sequenceVertices += addQuad(getGlyphQuad(glyphIndex, x, y), z, sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addGlyph(int glyphIndex, float x, float y, float z, const Rectf &clip)
    {
        if (glyphIndex >= 0)
        {
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += addQuad(quad, z, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph2D(int glyphIndex, float x, float y)
    {
        if (glyphIndex >= 0)
        {
            sequenceVertices += matrix.addTransformedQuad2D(getGlyphQuad(glyphIndex, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addTransformedGlyph2D(int glyphIndex, float x, float y, const Rectf &clip)
    {
        if (glyphIndex >= 0)
        {
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += matrix.addTransformedQuad2D(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
    
    void XFont::addTransformedGlyph3D(int glyphIndex, float x, float y)
    {
        if (glyphIndex >= 0)
        {
            sequenceVertices += matrix.addTransformedQuad3D(getGlyphQuad(glyphIndex, x, y), sequenceVertices);
            incrementSequence();
        }
    }
    
    void XFont::addTransformedGlyph3D(int glyphIndex, float x, float y, const Rectf &clip)
    {
        if (glyphIndex >= 0)
        {
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (computeClip(quad, clip))
            {
                sequenceVertices += matrix.addTransformedQuad3D(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
}
