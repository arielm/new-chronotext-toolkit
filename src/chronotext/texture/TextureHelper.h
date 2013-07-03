/*
 * TODO: USE ci::gl::TextureRef INSTEAD OF ci::gl::Texture
 */

#pragma once

#include "chronotext/texture/TextureData.h"

class TextureHelper
{
    static TextureData getTranslucentTextureData(InputSourceRef inputSource, ci::gl::Texture::Format &format);
    static TextureData getPowerOfTwoTextureData(InputSourceRef inputSource, ci::gl::Texture::Format &format);

public:
    enum
    {
        FLAGS_NONE = 0,
        FLAGS_TRANSLUCENT = 1,
        FLAGS_POT = 2
    };
    
    static ci::gl::Texture* loadTexture(const std::string &resourceName, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    static ci::gl::Texture* loadTexture(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);

    static TextureData getTextureData(InputSourceRef inputSource, bool useMipmap = false, int flags = FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    static ci::gl::Texture* uploadTexture(const TextureData &textureData);
    
    static void unloadTexture(ci::gl::Texture *texture);
    static void bindTexture(ci::gl::Texture *texture);
    
    static void beginTexture(ci::gl::Texture *texture);
    static void endTexture();
    
    static void drawTextureFromCenter(ci::gl::Texture *texture);
    static void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
    static void drawTextureInRect(ci::gl::Texture *texture, const ci::Rectf &rect, float ox = 0, float oy = 0);
};
