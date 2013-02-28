#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

typedef boost::shared_ptr<class ci::gl::Texture> TextureRef;

class TextureHelper
{
public:
    enum
    {
        FILTER_NONE,
        FILTER_TRANSLUCENT
    };
    
    static ci::gl::Texture* loadTexture(const std::string &resourceName, bool useMipmap = false, int filter = FILTER_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    static ci::gl::Texture* loadTexture(InputSourceRef inputSource, bool useMipmap = false, int filter = FILTER_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    static void deleteTexture(ci::gl::Texture *texture);
    static void bindTexture(ci::gl::Texture *texture);
    
    static void beginTexture(ci::gl::Texture *texture);
    static void endTexture();
    
    static void drawTextureFromCenter(ci::gl::Texture *texture);
    static void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
};
