#pragma once

#include "chronotext/texture/TextureData.h"

namespace chronotext
{
    class TextureHelper
    {
        static void textureDeallocator(void *refcon);
        static TextureData fetchTranslucentTextureData(const TextureRequest &textureRequest);
        static TextureData fetchPowerOfTwoTextureData(const TextureRequest &textureRequest);
        
    public:
        static ci::gl::TextureRef loadTexture(const std::string &resourceName, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
        static ci::gl::TextureRef loadTexture(InputSourceRef inputSource, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
        static ci::gl::TextureRef loadTexture(const TextureRequest &textureRequest);
        
        static TextureData fetchTextureData(const TextureRequest &textureRequest);
        static ci::gl::TextureRef uploadTextureData(const TextureData &textureData);
        
        static void bindTexture(ci::gl::Texture *texture);
        static void beginTexture(ci::gl::Texture *texture);
        static void endTexture();
        
        static void drawTextureFromCenter(ci::gl::Texture *texture);
        static void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
        static void drawTextureInRect(ci::gl::Texture *texture, const ci::Rectf &rect, float ox = 0, float oy = 0);
    };
}

namespace chr = chronotext;
