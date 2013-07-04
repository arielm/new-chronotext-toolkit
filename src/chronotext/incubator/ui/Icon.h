#pragma once

#include "chronotext/texture/Texture.h"

namespace chronotext
{
    typedef std::shared_ptr<class Icon> IconRef;
    
    class Icon
    {
        float width;
        float height;
        
    public:
        TextureRef texture;
        float scale;
        
        Icon(TextureRef texture, float scale = 1, float width = -1, float height = -1);
        
        float getWidth();
        float getHeight();
    };
};

namespace chr = chronotext;
