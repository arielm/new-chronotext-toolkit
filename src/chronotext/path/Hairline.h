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
        
        Hairline();
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
