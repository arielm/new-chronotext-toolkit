/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

namespace chr
{
    struct TextureRequest
    {
        enum Flags
        {
            FLAGS_NONE = 0,
            FLAGS_TRANSLUCENT = 1,
            FLAGS_POT = 2
        };
        
        InputSource::Ref inputSource;
        bool useMipmap;
        Flags flags;
        GLenum wrapS;
        GLenum wrapT;
        ci::Vec2i maxSize;
        
        TextureRequest()
        :
        inputSource(InputSource::Ref()),
        useMipmap(false),
        flags(FLAGS_NONE),
        wrapS(0),
        wrapT(0)
        {}
        
        TextureRequest(InputSource::Ref inputSource, bool useMipmap = false, Flags flags = FLAGS_NONE)
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
        
        TextureRequest& setWrap(GLenum s, GLenum t)
        {
            wrapS = s;
            wrapT = t;
            return *this;
        }
        
        TextureRequest& setMaxSize(const ci::Vec2i &size)
        {
            maxSize = size;
            return *this;
        }
        
        bool operator<(const TextureRequest &rhs) const
        {
            if (std::tie(useMipmap, flags, wrapS, wrapT) == std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT))
            {
                return (inputSource->getURI() < rhs.inputSource->getURI());
            }
            else
            {
                return std::tie(useMipmap, flags, wrapS, wrapT) < std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT);
            }
        }
    };
}
