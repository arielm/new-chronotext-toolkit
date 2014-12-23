/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/incubator/ui/Icon.h"

using namespace ci;

namespace chr
{
    Icon::Icon(Texture::Ref texture, float scale, float width, float height)
    :
    texture(texture),
    scale(scale)
    {
        this->width = (width < 0) ? texture->getCleanWidth() : width;
        this->height = (height < 0) ? width = texture->getCleanHeight() : height;
    }
    
    float Icon::getWidth()
    {
        return width * scale;
    }
    
    float Icon::getHeight()
    {
        return height * scale;
    }
}
