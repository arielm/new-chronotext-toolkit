/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"

#include <map>

namespace chr
{
    class TextureManager
    {
    public:
        Texture::Ref getTexture(InputSource::Ref inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        Texture::Ref getTexture(const TextureRequest &textureRequest);

        void discardTexture(Texture::Ref texture);
        bool reloadTexture(Texture::Ref texture);
        
        void discardTextures();
        void reloadTextures();
        
    protected:
        std::map<TextureRequest, Texture::Ref> textures;
    };
}
