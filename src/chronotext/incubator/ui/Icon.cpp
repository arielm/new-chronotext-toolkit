#include "chronotext/incubator/ui/Icon.h"

using namespace ci;

namespace chronotext
{
    Icon::Icon(Texture *texture, float scale, float width, float height)
    :
    texture(texture),
    scale(scale)
    {
        this->width = (width < 0) ? texture->getWidth() : width;
        this->height = (height < 0) ? width = texture->getHeight() : height;
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
