/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/StrokeHelper.h"

namespace chr
{
    class Hairline
    {
    public:
        enum Type
        {
            TYPE_NORMAL,
            TYPE_DASHED
        };
        
        Hairline() = default;
        Hairline(TextureManager &textureManager, Type type);
        
        void stroke(const FollowablePath &path, float scale = 1, float uOffset = 0);
        void stroke(const FollowablePath &path, const std::vector<float> &offsets, float scale = 1, float uOffset = 0);
        
        void draw();
        
    protected:
        Type type;
        
        Texture::Ref texture;
        TexturedTriangleStrip strip;
    };
}
