#pragma once

#include "chronotext/texture/TextureManager.h"
#include "chronotext/path/StrokeHelper.h"

namespace chronotext
{
    class Hairline
    {
    public:
        typedef enum
        {
            TYPE_NORMAL,
            TYPE_DASHED
        }
        Type;
        
        Hairline();
        Hairline(TextureManager &textureManager, Type type);
        
        void stroke(const FollowablePath &path, float scale = 1, float uOffset = 0);
        void stroke(const FollowablePath &path, const std::vector<float> &offsets, float scale = 1, float uOffset = 0);
        
        void draw();
        
    protected:
        Type type;
        
        TextureRef texture;
        TexturedTriangleStrip strip;
    };
}

namespace chr = chronotext;
