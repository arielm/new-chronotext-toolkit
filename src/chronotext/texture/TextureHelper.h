/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

/*
 * TODO:
 *
 * 1) CHECK TextureHelper::uploadTextureData() WITH "TYPE_IMAGE_SOURCE", AS IT MAY CONSUME MORE MEMORY THAN NECESSARY
 *
 * 2) MEMORY-PROBE TEXTURE-DISCARDING
 *
 * 3) MOVE LOGGING AND "SECOND PART" OF MEMORY-PROBING FROM TextureHelper TO Texture
 */

#pragma once

#include "chronotext/texture/TextureData.h"

namespace chr
{
    class TextureHelper
    {
    public:
        static bool VERBOSE;
        static bool PROBE_MEMORY;

        static ci::gl::TextureRef loadTexture(const std::string &resourceName, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        static ci::gl::TextureRef loadTexture(InputSourceRef inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        static ci::gl::TextureRef loadTexture(const TextureRequest &textureRequest);
        
        static TextureData fetchTextureData(const TextureRequest &textureRequest);
        static ci::gl::TextureRef uploadTextureData(const TextureData &textureData);
        
        static void bindTexture(ci::gl::Texture *texture);
        static void beginTexture(ci::gl::Texture *texture);
        static void endTexture();
        
        static void drawTextureFromCenter(ci::gl::Texture *texture);
        static void drawTexture(ci::gl::Texture *texture, float rx = 0, float ry = 0);
        static void drawTextureInRect(ci::gl::Texture *texture, const ci::Rectf &rect, float ox = 0, float oy = 0);
        
    protected:
        static void textureDeallocator(void *refcon);
        static TextureData fetchTranslucentTextureData(const TextureRequest &textureRequest);
        static TextureData fetchPowerOfTwoTextureData(const TextureRequest &textureRequest);
    };
}
