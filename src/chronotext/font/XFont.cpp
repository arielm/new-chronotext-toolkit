/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/XFont.h"
#include "chronotext/font/FontManager.h"

using namespace ci;
using namespace std;

namespace chronotext
{
    XFont::XFont(FontData *data, FontTexture *texture, const vector<GLushort> &indices, const Properties &properties)
    :
    texture(texture),
    indices(indices),
    properties(properties),
    began(0),
    hasClip(false),
    colors(NULL),
    sequence(NULL)
    {
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
        
        textureWidth = texture->width;
        textureHeight = texture->height;
        
        // ---
        
        vertices = new float[properties.slotCapacity * (properties.maxDimensions + 2) * 4];
        
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
        setColor(ColorA(0, 0, 0, 1));
    }
    
    XFont::~XFont()
    {
        delete[] vertices;
        
        if (colors)
        {
            delete[] colors;
        }
    }
    
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
                return -1; // SHALL WE USE A "MISSING GLYPH" AND RETURN ITS INDEX?
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

    void XFont::setColor(const ColorA &color)
    {
        this->color = color;
    }
    
    void XFont::setColor(float r, float g, float b, float a)
    {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }
    
    void XFont::setStrikethroughFactor(float factor)
    {
        strikethroughFactor = factor;
    }

    void XFont::setClip(const Rectf &clip)
    {
        this->clip = clip;
        hasClip = true;
    }
    
    void XFont::setClip(float x1, float y1, float x2, float y2)
    {
        clip.x1 = x1;
        clip.y1 = y1;
        clip.x2 = x2;
        clip.y2 = y2;
        
        hasClip = true;
    }
    
    void XFont::clearClip()
    {
        hasClip = false;
    }
    
    float XFont::getSize() const
    {
        return size;
    }
    
    float XFont::getDirection() const
    {
        return direction * axis.x;
    }
    
    const Vec2f& XFont::getAxis() const
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
    
    float XFont::getOffsetX(const wstring &text, Alignment align) const
    {
        switch (align)
        {
            case ALIGN_MIDDLE:
                return -0.5f * getStringAdvance(text);
                
            case ALIGN_RIGHT:
                return -getStringAdvance(text);
                
            default:
                return 0;
        }
    }
    
    float XFont::getOffsetY(Alignment align) const
    {
        switch (align)
        {
            case ALIGN_MIDDLE:
                return getStrikethroughOffset();
                
            case ALIGN_TOP:
                return +getAscent();
                
            case ALIGN_BOTTOM:
                return -getDescent();
                
            default:
                return 0;
        }
    }
    
    FontMatrix* XFont::getMatrix()
    {
        return &matrix;
    }
    
    const GLushort* XFont::getIndices() const
    {
        return const_cast<GLushort*>(indices.data());
    }
    
    void XFont::begin(bool useColor)
    {
        if (began == 0)
        {
            texture->reload();
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture->name);
            
            if (properties.useAnisotropy && anisotropyAvailable)
            {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
            }
            
            if (useColor)
            {
                glEnableClientState(GL_COLOR_ARRAY);
            }
            else
            {
                gl::color(color);
            }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        
        began++;
    }
    
    void XFont::end(bool useColor)
    {
        began--;
        
        if (began == 0)
        {
            if (properties.useAnisotropy && anisotropyAvailable)
            {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
            }
            
            glDisable(GL_TEXTURE_2D);
            
            if (useColor)
            {
                glDisableClientState(GL_COLOR_ARRAY);
            }
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
    
    void XFont::beginSequence(XFontSequence *sequence, int dimensions, bool useColor)
    {
        sequenceDimensions = dimensions;
        sequenceUseColor = useColor;

        if (useColor && !colors)
        {
            colors = new ColorA[properties.slotCapacity * 4];
        }
        
        sequenceSize = 0;
        sequenceVertices = vertices;
        sequenceColors = colors;

        if (sequence)
        {
            this->sequence = sequence;
            sequence->begin(this, properties.slotCapacity, dimensions, useColor);
        }
        else
        {
            begin(useColor);
        }
        
        clearClip();
    }
    
    void XFont::endSequence()
    {
        if (sequence)
        {
            sequence->flush(sequenceSize, vertices, colors);
            sequence->end();
            sequence = NULL;
        }
        else
        {
            flush(sequenceSize);
            end(sequenceUseColor);
        }
    }
    
    void XFont::flush(int count)
    {
        int stride = sizeof(float) * (sequenceDimensions + 2);
        
        if (sequenceUseColor)
        {
            glColorPointer(4, GL_FLOAT, 0, colors);
        }

        glVertexPointer(sequenceDimensions, GL_FLOAT, stride, vertices);
        glTexCoordPointer(2, GL_FLOAT, stride, vertices + sequenceDimensions);
        glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, indices.data());
    }
    
    void XFont::incrementSequence()
    {
        if (sequenceUseColor)
        {
            *sequenceColors++ = color;
            *sequenceColors++ = color;
            *sequenceColors++ = color;
            *sequenceColors++ = color;
        }

        sequenceSize++;
        
        if (sequenceSize == properties.slotCapacity)
        {
            if (sequence)
            {
                sequence->flush(sequenceSize, vertices, colors);
            }
            else
            {
                flush(sequenceSize);
            }
            
            sequenceSize = 0;
            sequenceVertices = vertices;
            sequenceColors = colors;
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
    
    bool XFont::clipQuad(GlyphQuad &quad)
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
                quad.u1 += axis.x * dx / textureWidth / sizeRatio;
            }
            
            if (quad.x2 > clip.x2)
            {
                float dx = clip.x2 - quad.x2;
                quad.x2 += dx;
                quad.u2 += axis.x * dx / textureWidth / sizeRatio;
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
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (!hasClip || clipQuad(quad))
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
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (!hasClip || clipQuad(quad))
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
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (!hasClip || clipQuad(quad))
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
            GlyphQuad quad = getGlyphQuad(glyphIndex, x, y);
            
            if (!hasClip || clipQuad(quad))
            {
                sequenceVertices += matrix.addTransformedQuad3D(quad, sequenceVertices);
                incrementSequence();
            }
        }
    }
}
