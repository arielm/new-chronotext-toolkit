/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/Texture.h"

namespace chr
{
    typedef std::shared_ptr<class Icon> IconRef;
    
    class Icon
    {
    public:
        Texture::Ref texture;
        float scale;
        
        Icon(Texture::Ref texture, float scale = 1, float width = -1, float height = -1);
        
        float getWidth();
        float getHeight();
        
    protected:
        float width;
        float height;
    };
};
