#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/texture/TextureRequest.h"

#include "cinder/Surface.h"

struct TextureData
{
    enum
    {
        TYPE_SURFACE,
        TYPE_IMAGE_SOURCE,
        TYPE_PVR,
        TYPE_DATA
    };
    
    bool defined;
    int type;
    TextureRequest request;
    
    ci::Surface surface;
    ci::ImageSourceRef imageSource;
    ci::Buffer buffer;
    std::shared_ptr<uint8_t> data;
    
    float maxU;
    float maxV;

    GLenum glInternalFormat;
    GLenum glFormat;
    int width;
    int height;
    
    TextureData()
    :
    defined(false)
    {}
    
    TextureData(const TextureRequest &request, const ci::Surface &surface, float maxU = 1, float maxV = 1)
    :
    defined(true),
    type(TYPE_SURFACE),
    request(request),
    surface(surface),
    maxU(maxU),
    maxV(maxV)
    {}
    
    TextureData(const TextureRequest &request, ci::ImageSourceRef imageSource)
    :
    defined(true),
    type(TYPE_IMAGE_SOURCE),
    request(request),
    imageSource(imageSource)
    {}
    
    TextureData(const TextureRequest &request, const ci::Buffer &buffer)
    :
    defined(true),
    type(TYPE_PVR),
    request(request),
    buffer(buffer)
    {}
    
    TextureData(const TextureRequest &request, std::shared_ptr<uint8_t> data, GLenum glInternalFormat, GLenum glFormat, int width, int height)
    :
    defined(true),
    type(TYPE_DATA),
    request(request),
    data(data),
    glInternalFormat(glInternalFormat),
    glFormat(glFormat),
    width(width),
    height(height)
    {}
    
    bool undefined() const
    {
        return !defined;
    }
};
