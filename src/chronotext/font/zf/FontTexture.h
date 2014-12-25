/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/GlyphData.h"

#include "hb.h"

namespace chr
{
    namespace zf
    {
        class ActualFont;

        class FontTexture
        {
        public:
            ActualFont *font;
            hb_codepoint_t codepoint;
            
            bool useMipmap;
            int width;
            int height;
            uint32_t glId;

            FontTexture(ActualFont *font, hb_codepoint_t codepoint, const GlyphData &glyphData);
            ~FontTexture();
            
            void upload(const GlyphData &glyphData);
            void discard();
            bool reload();
            void bind();
            
            size_t getMemoryUsage() const;
            inline ci::Vec2i getSize() const { return ci::Vec2i(width, height); }
        };
    }
}
