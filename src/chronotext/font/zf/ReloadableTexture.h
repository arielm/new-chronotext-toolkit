/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/zf/GlyphData.h"

#include "cinder/gl/gl.h"

namespace chronotext
{
    namespace zf
    {
        class ReloadableTexture
        {
        public:
            bool useMipmap;
            int width;
            int height;
            GLuint id;

            ReloadableTexture(const GlyphData &glyphData);
            ~ReloadableTexture();
            
            void upload(const GlyphData &glyphData);
            void discard();
            
            bool isLoaded() const;
            size_t getMemoryUsage() const;
            ci::Vec2i getSize() const;
        };
    }
}

namespace chr = chronotext;
