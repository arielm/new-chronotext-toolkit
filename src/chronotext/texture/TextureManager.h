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
 * 1) INCORPORATE TextureRequest INTO Texture
 *    - SHOULD BE: Texture::Request
 *    - SIMILAR TO Effect::Request
 *
 * 2) ADD int tag TO TextureManager
 *    - DEFAULT-VALUE: 0
 *    - WILL ALLOW TO MAINTAIN SEVERAL "SETS OF TEXTURES"
 *    - REQUIRED CHANGES TO THE API:
 *      - TextureManager:
 *        - void discardTextures(int tag = 0)
 *        - bool reloadTextures(int tag = 0)
 *
 * 3) AVOID IF POSSIBLE:
 *    - "EMPTY" Texture::Request
 *    = "EMPTY" TextureData
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
