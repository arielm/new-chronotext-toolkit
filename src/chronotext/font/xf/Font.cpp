/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/xf/Font.h"
#include "chronotext/font/xf/FontManager.h"

using namespace ci;
using namespace std;

namespace chronotext
{
    namespace xf
    {
        Font::Font(FontManager &fontManager, FontData *data, FontTexture *texture, const Properties &properties)
        :
        texture(texture),
        properties(properties),
        indices(fontManager.getIndices(properties.slotCapacity)),
        began(0),
        hasClip(false),
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
            
            anisotropyAvailable = gl::isExtensionAvailable("GL_EXT_texture_filter_anisotropic");
            
            if (anisotropyAvailable)
            {
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            }
            
            // ---
            
            setSize(nativeFontSize);
            setDirection(+1);
            setAxis(Vec2f(+1, +1));
            setColor(0, 0, 0, 1);
        }
        
        bool Font::isSpace(wchar_t c) const
        {
            return (c == 0x20) || (c == 0xa0);
        }
        
        bool Font::isValid(wchar_t c) const
        {
            return glyphs.count(c);
        }
        
        int Font::getGlyphIndex(wchar_t c) const
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
        
        wstring Font::getCharacters() const
        {
            wstring characters;
            characters.reserve(glyphs.size());
            
            for (auto it : glyphs)
            {
                characters.push_back(it.first);
            }
            
            return characters;
        }
        
        void Font::setSize(float size)
        {
            this->size = size;
            sizeRatio = size / nativeFontSize;
        }
        
        void Font::setDirection(float direction)
        {
            this->direction = direction;
        }
        
        void Font::setAxis(const Vec2f &axis)
        {
            this->axis = axis;
        }
        
        void Font::setColor(const ColorA &color)
        {
            this->color = color;
        }
        
        void Font::setColor(float r, float g, float b, float a)
        {
            color.r = r;
            color.g = g;
            color.b = b;
            color.a = a;
        }
        
        void Font::setStrikethroughFactor(float factor)
        {
            strikethroughFactor = factor;
        }
        
        void Font::setClip(const Rectf &clip)
        {
            this->clip = clip;
            hasClip = true;
        }
        
        void Font::setClip(float x1, float y1, float x2, float y2)
        {
            clip.x1 = x1;
            clip.y1 = y1;
            clip.x2 = x2;
            clip.y2 = y2;
            
            hasClip = true;
        }
        
        void Font::clearClip()
        {
            hasClip = false;
        }
        
        float Font::getSize() const
        {
            return size;
        }
        
        float Font::getDirection() const
        {
            return direction * axis.x;
        }
        
        const Vec2f& Font::getAxis() const
        {
            return axis;
        }
        
        float Font::getGlyphAdvance(int glyphIndex) const
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
        
        float Font::getCharAdvance(wchar_t c) const
        {
            return getGlyphAdvance(getGlyphIndex(c));
        }
        
        float Font::getStringAdvance(const wstring &s) const
        {
            return getSubStringAdvance(s, 0, s.size());
        }
        
        float Font::getSubStringAdvance(const wstring &s, int begin, int end) const
        {
            float advance = 0;
            
            for (int i = begin; i < end; i++)
            {
                advance += getCharAdvance(s.at(i));
            }
            
            return advance;
        }
        
        float Font::getHeight() const
        {
            return height * sizeRatio;
        }
        
        float Font::getAscent() const
        {
            return ascent * sizeRatio;
        }
        
        float Font::getDescent() const
        {
            return descent * sizeRatio;
        }
        
        float Font::getStrikethroughOffset() const
        {
            return strikethroughFactor * (ascent - descent) * sizeRatio;
        }
        
        float Font::getUnderlineOffset() const
        {
            return underlineOffset * sizeRatio;
        }
        
        float Font::getLineThickness() const
        {
            return lineThickness * sizeRatio;
        }
        
        float Font::getOffsetX(const wstring &text, Alignment align) const
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
        
        float Font::getOffsetY(Alignment align) const
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
        
        FontMatrix* Font::getMatrix()
        {
            return &matrix;
        }
        
        const GLushort* Font::getIndices() const
        {
            return const_cast<GLushort*>(indices.data());
        }
        
        void Font::begin(bool useColor)
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
        
        void Font::end(bool useColor)
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
        
        void Font::beginSequence(FontSequence *sequence, bool useColor)
        {
            sequenceUseColor = useColor;

            clearClip();
            glyphSequence.clear();
            
            if (sequence)
            {
                this->sequence = sequence;
                sequence->begin(useColor);
            }
            else
            {
                begin(useColor);
            }
        }
        
        void Font::endSequence()
        {
            if (sequence)
            {
                sequence->flush(glyphSequence);
                sequence->end();
                sequence = NULL;
            }
            else
            {
                glyphSequence.flush(getIndices(), sequenceUseColor);
                end(sequenceUseColor);
            }
        }
        
        void Font::incrementSequence()
        {
            if (sequenceUseColor)
            {
                glyphSequence.addColor(color);
            }
            
            if (glyphSequence.size() == properties.slotCapacity)
            {
                if (sequence)
                {
                    sequence->flush(glyphSequence);
                }
                else
                {
                    glyphSequence.flush(getIndices(), sequenceUseColor);
                }
                
                glyphSequence.clear();
            }
        }
        
        GlyphQuad Font::obtainQuad(int glyphIndex, float x, float y) const
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
        
        bool Font::clipQuad(GlyphQuad &quad)
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
        
        void Font::addGlyph(int glyphIndex, float x, float y, float z)
        {
            if (glyphIndex >= 0)
            {
                GlyphQuad quad = obtainQuad(glyphIndex, x, y);
                
                if (!hasClip || clipQuad(quad))
                {
                    glyphSequence.addQuad(quad);
                    incrementSequence();
                }
            }
        }
        
        void Font::addTransformedGlyph(int glyphIndex, float x, float y)
        {
            if (glyphIndex >= 0)
            {
                GlyphQuad quad = obtainQuad(glyphIndex, x, y);
                
                if (!hasClip || clipQuad(quad))
                {
                    matrix.addTransformedQuad(quad, glyphSequence.vertices);
                    incrementSequence();
                }
            }
        }
    }
}
