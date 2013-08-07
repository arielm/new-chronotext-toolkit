#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

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
    
    TextureRequest()
    {}
    
    TextureRequest(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE)
    :
    inputSource(inputSource),
    useMipmap(useMipmap),
    flags(flags),
    wrapS(wrapS),
    wrapT(wrapT)
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
    
    bool operator==(const TextureRequest &rhs) const
    {
        return (inputSource->getUniqueName() == rhs.inputSource->getUniqueName()) && (useMipmap == rhs.useMipmap) && (flags == rhs.flags) && (wrapS == rhs.wrapS) && (wrapT == rhs.wrapT);
    }
};
