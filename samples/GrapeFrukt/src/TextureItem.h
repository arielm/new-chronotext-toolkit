#pragma once

#include "Frame.h"

#include <vector>
#include <string>

struct TextureItem
{
    std::string path;
    float width;
    float height;
    float registrationPointX;
    float registrationPointY;
    int zIndex;
    
    std::vector<Frame> frames;
    
    TextureItem(const std::string &path, float width, float height, float registrationPointX, float registrationPointY, float zIndex)
    :
    path(path),
    width(width),
    height(height),
    registrationPointX(registrationPointX),
    registrationPointY(registrationPointY),
    zIndex(zIndex)
    {}
    
    static bool compareZIndex(TextureItem *lhs, TextureItem *rhs)
    {
        return (lhs->zIndex < rhs->zIndex);
    }
};
