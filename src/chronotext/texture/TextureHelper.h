#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

class TextureHelper
{
public:
    enum
    {
        FLAGS_NONE = 0,
        FLAGS_TRANSLUCENT = 1,
        FLAGS_POT = 2
    };
    
    static ci::gl::Texture* loadTexture(const std::string &resourceName, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    static ci::gl::Texture* loadTexture(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    static void deleteTexture(ci::gl::Texture *texture);
    static void bindTexture(ci::gl::Texture *texture);
    
    static void beginTexture(ci::gl::Texture *texture);
    static void endTexture();
    
    static void drawTextureFromCenter(ci::gl::Texture *texture);
    static void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
    static void drawTextureInRect(ci::gl::Texture *texture, const ci::Rectf &rect, float ox = 0, float oy = 0);
    
    static ci::gl::Texture* getTranslucentTexture(ci::DataSourceRef source, ci::gl::Texture::Format &format);
    static ci::gl::Texture* getPowerOfTwoTexture(ci::DataSourceRef source, ci::gl::Texture::Format &format);
};
