/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"

#include <map>

namespace chronotext
{
    class TextureManager
    {
    public:
        TextureManager();
        
        TextureRef getTexture(const std::string &resourceName, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE);
        TextureRef getTexture(InputSourceRef inputSource, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE);
        TextureRef getTexture(const TextureRequest &textureRequest);
        
        bool remove(TextureRef texture);
        void clear();
        
        void unload();
        void reload();
        
    protected:
        std::map<TextureRequest, TextureRef> cache;
        bool unloaded;
    };
}

namespace chr = chronotext;
