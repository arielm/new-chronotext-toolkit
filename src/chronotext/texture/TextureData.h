/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/texture/TextureRequest.h"
#include "chronotext/texture/PVRHelper.h"

#include "cinder/Surface.h"
#include "cinder/ImageIo.h"

namespace chr
{
    struct TextureData
    {
        enum Type
        {
            TYPE_SURFACE,
            TYPE_IMAGE_SOURCE,
            TYPE_PVR,
            TYPE_DATA
        };
        
        bool defined;
        Type type;
        TextureRequest request;
        
        /*
         * NO SPECIAL-TREATMENT IS NECESSARY FOR surface AND buffer IN ORDER TO AVOID EXTRA DATA-COPYING
         * SINCE ci::Surface AND ci::Buffer EMULATE shared_ptr INTERNALLY
         */
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
}
