/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/font/zf/FontTexture.h"
#include "chronotext/font/zf/ActualFont.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace zf
    {
        FontTexture::FontTexture(ActualFont *font, hb_codepoint_t codepoint, const GlyphData &glyphData)
        :
        font(font),
        codepoint(codepoint),
        id(0)
        {
            upload(glyphData);
        }
        
        FontTexture::~FontTexture()
        {
            discard();
        }
        
        void FontTexture::upload(const GlyphData &glyphData)
        {
            if (!id && glyphData.isValid())
            {
                auto glyphWidth = glyphData.width;
                auto glyphHeight = glyphData.height;
                auto glyphPadding = glyphData.padding;
                auto buffer = glyphData.getBuffer();
                
                useMipmap = glyphData.useMipmap;
                width = nextPowerOfTwo(glyphWidth + glyphPadding * 2);
                height = nextPowerOfTwo(glyphHeight + glyphPadding * 2);
                
                auto data = new unsigned char[width * height](); // ZERO-FILLED
                
                for (int y = 0; y < glyphHeight; y++)
                {
                    for (int x = 0; x < glyphWidth; x++)
                    {
                        data[(y + glyphPadding) * width + (x + glyphPadding)] = buffer[y * glyphWidth + x];
                    }
                }
                
                // ---
                
                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);
                
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
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
                
                if (useMipmap)
                {
                    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
                }
                
                glBindTexture(GL_TEXTURE_2D, 0);
                delete[] data;
            }
        }
        
        void FontTexture::discard()
        {
            if (id)
            {
                glDeleteTextures(1, &id);
                id = 0;
            }
        }
        
        void FontTexture::reload()
        {
            if (!id)
            {
                font->reloadTexture(this);
            }
        }
        
        void FontTexture::bind()
        {
            reload();
            glBindTexture(GL_TEXTURE_2D, id);
        }
        
        size_t FontTexture::getMemoryUsage() const
        {
            if (id)
            {
                if (useMipmap)
                {
                    return size_t(width * height * 1.333f);
                }
                else
                {
                    return width * height;
                }
            }
            else
            {
                return 0;
            }
        }
    }
}
