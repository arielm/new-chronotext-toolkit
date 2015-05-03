/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014-2015, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * WARNING:
 * IF THE GlyphData IS NOT FOR "IMMEDIATE CONSUMPTION", INVOKE copyDataAndReleaseSlot()
 * OTHERWISE, THE DATA WILL BECOME CORRUPTED UPON THE NEXT FT_Get_Glyph() OPERATION
 */

#pragma once

#include "chronotext/font/zf/FreetypeHelper.h"

#include "cinder/Vector.h"

namespace chr
{
    namespace zf
    {
        class GlyphData
        {
        public:
            bool useMipmap;
            int padding;
            
            int width;
            int height;
            ci::Vec2f offset;
            ci::Vec2f size;
            
            GlyphData(FT_Face ftFace, FT_UInt codepoint, bool useMipmap, int padding)
            :
            useMipmap(useMipmap),
            padding(padding)
            {
                if (codepoint > 0)
                {
                    if (!FT_Load_Glyph(ftFace, codepoint, FT_LOAD_DEFAULT | FT_LOAD_FORCE_AUTOHINT))
                    {
                        ftSlot = ftFace->glyph;
                        
                        if (!FT_Get_Glyph(ftSlot, &ftGlyph))
                        {
                            FT_Render_Glyph(ftSlot, FT_RENDER_MODE_NORMAL);
                            
                            width = ftSlot->bitmap.width;
                            height = ftSlot->bitmap.rows;
                            
                            if (width * height > 0)
                            {
                                offset = ci::Vec2f(ftSlot->bitmap_left, -ftSlot->bitmap_top) - ci::Vec2f(padding, padding);
                                size = ci::Vec2f(width, height) + ci::Vec2f(padding, padding) * 2;
                            }
                            else
                            {
                                FT_Done_Glyph(ftGlyph);
                                ftGlyph = nullptr;
                            }
                        }
                    }
                }
            }
            
            ~GlyphData()
            {
                if (ftGlyph)
                {
                    FT_Done_Glyph(ftGlyph);
                }
                else if (data)
                {
                    free(data);
                }
            }
            
            bool isValid() const
            {
                return (ftGlyph || data);
            }
            
            unsigned char* getBuffer() const
            {
                if (ftGlyph)
                {
                    return ftSlot->bitmap.buffer;
                }
                
                if (data)
                {
                    return data;
                }

                return nullptr;
            }
            
            void copyDataAndReleaseSlot()
            {
                if (ftGlyph)
                {
                    size_t dataSize = width * height;
                    data = (unsigned char*)malloc(dataSize);
                    memcpy(data, ftSlot->bitmap.buffer, dataSize);
                    
                    FT_Done_Glyph(ftGlyph);
                    ftGlyph = nullptr;
                }
            }
            
        protected:
            FT_GlyphSlot ftSlot;
            FT_Glyph ftGlyph = nullptr;
            unsigned char *data = nullptr;
        };
    }
}
