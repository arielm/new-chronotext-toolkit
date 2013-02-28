#pragma once

#include "chronotext/texture/Texture.h"
#include "chronotext/utils/Utils.h"

namespace chronotext
{
    typedef boost::shared_ptr<class Icon> IconRef;
    
    class Icon
    {
        float width;
        float height;
        
    public:
        Texture *texture;
        float scale;
        
        Icon(Texture *texture, float scale = 1, float width = -1, float height = -1);
        
        ~Icon()
        {
            DLOG("Icon DELETED");
        }
        
        float getWidth();
        float getHeight();
    };
};

namespace chr = chronotext;
