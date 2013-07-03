#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

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
    InputSourceRef inputSource;
    ci::gl::Texture::Format format;
    
    ci::Surface surface;
    ci::ImageSourceRef imageSource;
    ci::Buffer buffer;
    std::shared_ptr<uint8_t> data;
    
    float maxU;
    float maxV;
    
    GLenum dataFormat;
    int width;
    int height;
    
    TextureData()
    :
    defined(false)
    {}
    
    TextureData(InputSourceRef inputSource, const ci::gl::Texture::Format &format, const ci::Surface &surface, float maxU = 1, float maxV = 1)
    :
    defined(true),
    type(TYPE_SURFACE),
    inputSource(inputSource),
    format(format),
    surface(surface),
    maxU(maxU),
    maxV(maxV)
    {}

    TextureData(InputSourceRef inputSource, const ci::gl::Texture::Format &format, ci::ImageSourceRef imageSource)
    :
    defined(true),
    type(TYPE_IMAGE_SOURCE),
    inputSource(inputSource),
    format(format),
    imageSource(imageSource)
    {}
    
    TextureData(InputSourceRef inputSource, const ci::gl::Texture::Format &format, const ci::Buffer &buffer)
    :
    defined(true),
    type(TYPE_PVR),
    inputSource(inputSource),
    format(format),
    buffer(buffer)
    {}
    
    TextureData(InputSourceRef inputSource, const ci::gl::Texture::Format &format, std::shared_ptr<uint8_t> data, GLenum dataFormat, int width, int height)
    :
    defined(true),
    type(TYPE_DATA),
    inputSource(inputSource),
    format(format),
    data(data),
    dataFormat(dataFormat),
    width(width),
    height(height)
    {}
    
    bool undefined() const
    {
        return !defined;
    }
};
