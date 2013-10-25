/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

namespace chronotext
{
    struct TextureRequest
    {
        enum
        {
            FLAGS_NONE = 0,
            FLAGS_TRANSLUCENT = 1,
            FLAGS_POT = 2
        };
        
        InputSourceRef inputSource;
        bool useMipmap;
        int flags;
        GLenum wrapS;
        GLenum wrapT;
        ci::Vec2i maxSize;
        
        TextureRequest()
        {}
        
        TextureRequest(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE)
        :
        inputSource(inputSource),
        useMipmap(useMipmap),
        flags(flags),
        wrapS(GL_CLAMP_TO_EDGE),
        wrapT(GL_CLAMP_TO_EDGE),
        maxSize(0, 0)
        {}
        
        ci::gl::Texture::Format getFormat() const
        {
            ci::gl::Texture::Format format;
            format.setWrap(wrapS, wrapT);
            
            if (useMipmap)
            {
                format.enableMipmapping(true);
                format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
            }
            
            return format;
        }
        
        void setWrap(GLenum s, GLenum t)
        {
            wrapS = s;
            wrapT = t;
        }
        
        bool operator==(const TextureRequest &rhs) const
        {
            return (inputSource->getUniqueName() == rhs.inputSource->getUniqueName()) && (useMipmap == rhs.useMipmap) && (flags == rhs.flags) && (wrapS == rhs.wrapS) && (wrapT == rhs.wrapT);
        }
    };
}

namespace chr = chronotext;
