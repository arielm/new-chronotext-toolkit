/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"
#include "chronotext/texture/PVRHelper.h"
#include "chronotext/Context.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/ImageIo.h"
#include "cinder/ip/Fill.h"

using namespace std;
using namespace ci;

namespace chr
{
    bool TextureHelper::PROBE_MEMORY = true; // XXX
    
    MemoryInfo TextureHelper::memoryInfo[2];
    map<gl::Texture*, TextureHelper::MemoryProbe> TextureHelper::probes;

    // ---
    
    gl::TextureRef TextureHelper::loadTexture(const string &resourceName, bool useMipmap, TextureRequest::Flags flags)
    {
        return loadTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    gl::TextureRef TextureHelper::loadTexture(InputSource::Ref inputSource, bool useMipmap, TextureRequest::Flags flags)
    {
        return loadTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    gl::TextureRef TextureHelper::loadTexture(const TextureRequest &textureRequest)
    {
        TextureData textureData = fetchTextureData(textureRequest);
        
        if (textureData.undefined())
        {
            throw EXCEPTION(Texture, "TEXTURE IS UNDEFINED");
        }
        else
        {
            const Vec2i size = getTextureSize(textureData);
            
            if (isOverSized(textureRequest, size))
            {
                throw EXCEPTION(Texture, "TEXTURE IS OVER-SIZED (" + toString(size.x) + "x" + toString(size.y) + ")");
            }
        }

        return uploadTextureData(textureData);
    }
    
    // ---
    
    TextureData TextureHelper::fetchTextureData(const TextureRequest &textureRequest)
    {
        if (PROBE_MEMORY)
        {
            memoryInfo[0] = getMemoryInfo();
        }
        
        if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.gz"))
        {
            if (textureRequest.inputSource->isFile())
            {
                return TextureData(textureRequest, PVRHelper::decompressGZ(textureRequest.inputSource->getFilePath()));
            }
            else
            {
                throw EXCEPTION(Texture, "PVR.GZ TEXTURES CAN ONLY BE LOADED FROM FILES");
            }
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.ccz"))
        {
            return TextureData(textureRequest, PVRHelper::decompressCCZ(textureRequest.inputSource->loadDataSource()));
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr"))
        {
            return TextureData(textureRequest, textureRequest.inputSource->loadDataSource()->getBuffer());
        }
        else
        {
            if (textureRequest.flags & TextureRequest::FLAGS_TRANSLUCENT)
            {
                return TextureData(fetchTranslucentTextureData(textureRequest));
            }
            else if (textureRequest.flags & TextureRequest::FLAGS_POT)
            {
                return TextureData(fetchPowerOfTwoTextureData(textureRequest));
            }
            else
            {
                return TextureData(textureRequest, loadImage(textureRequest.inputSource->loadDataSource()));
            }
        }
        
        return TextureData();
    }
    
    gl::TextureRef TextureHelper::uploadTextureData(const TextureData &textureData)
    {
        gl::TextureRef texture;
        
        if (!textureData.undefined())
        {
            if (PROBE_MEMORY)
            {
                memoryInfo[1] = getMemoryInfo();
            }
            
            /*
             * NECESSARY IN ORDER TO CLEANUP EVENTUAL ERRORS
             */
            while(glGetError() != GL_NO_ERROR)
            {}

            auto format = textureData.request.getFormat();
            
            switch (textureData.type)
            {
                case TextureData::TYPE_SURFACE:
                    texture = gl::Texture::create(textureData.surface, format);
                    texture->setCleanTexCoords(textureData.maxU, textureData.maxV);
                    break;
                    
                case TextureData::TYPE_IMAGE_SOURCE:
                    texture = gl::Texture::create(textureData.imageSource, format);
                    break;
                    
                case TextureData::TYPE_PVR:
                    texture = PVRHelper::loadTexture(textureData.buffer, format.hasMipmapping(), format.getWrapS(), format.getWrapT());
                    break;
                    
                case TextureData::TYPE_DATA:
                    format.setInternalFormat(textureData.glInternalFormat);
                    texture = gl::Texture::create(textureData.data.get(), textureData.glFormat, textureData.width, textureData.height, format);
                    break;
            }
            
            if (glGetError() == GL_OUT_OF_MEMORY)
            {
                throw EXCEPTION(Texture, "GL: OUT-OF-MEMORY");
            }
            else if (texture)
            {
                auto key = texture.get();
                
                texture->setDeallocator(&TextureHelper::textureDeallocator, key);

                auto memoryUsage = getTextureMemoryUsage(textureData);
                probes[key] = MemoryProbe({textureData.request, memoryUsage, memoryInfo[0], memoryInfo[1]});
            }
        }
        
        return texture;
    }
    
    // ---
    
    void TextureHelper::textureDeallocator(void *refcon)
    {
        gl::Texture *texture = reinterpret_cast<gl::Texture*>(refcon);
        
        if (PROBE_MEMORY)
        {
            probes[texture].memoryInfo[2] = getMemoryInfo();
        }
    }
    
    bool TextureHelper::isOverSized(const TextureRequest &textureRequest, const Vec2i &size)
    {
        if ((textureRequest.maxSize.x > 0) && (textureRequest.maxSize.y > 0))
        {
            return (size.x > textureRequest.maxSize.x) || (size.y > textureRequest.maxSize.y);
        }
        
        return false;
    }
    
    /*
     * BASED ON https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/gl/Texture.cpp#L478-490
     */
    
    TextureData TextureHelper::fetchTranslucentTextureData(const TextureRequest &textureRequest)
    {
        Surface surface(loadImage(textureRequest.inputSource->loadDataSource()));
        
        Channel8u &channel = surface.getChannel(0);
        int width = channel.getWidth();
        int height = channel.getHeight();
        
        if (isOverSized(textureRequest, channel.getSize()))
        {
            return TextureData(textureRequest, nullptr, 0, 0, width, height);
        }
        else
        {
            shared_ptr<uint8_t> data;
            
            if ((channel.getIncrement() != 1) || (channel.getRowBytes() != width * sizeof(uint8_t)))
            {
                data = shared_ptr<uint8_t>(new uint8_t[width * height], checked_array_deleter<uint8_t>());
                uint8_t *dest = data.get();
                int8_t inc = channel.getIncrement();
                
                for (int y = 0; y < height; ++y)
                {
                    const uint8_t *src = channel.getData(0, y);
                    
                    for (int x = 0; x < width; ++x)
                    {
                        *dest++ = *src;
                        src += inc;
                    }
                }
            }
            else
            {
                data = shared_ptr<uint8_t>(channel.getData(), checked_array_deleter<uint8_t>());
            }
            
            return TextureData(textureRequest, data, GL_ALPHA, GL_ALPHA, width, height);
        }
    }
    
    TextureData TextureHelper::fetchPowerOfTwoTextureData(const TextureRequest &textureRequest)
    {
        /*
         * NO EXTRA DATA-COPYING WILL OCCUR BECAUSE ci::Surface
         * IS EMULATING shared_ptr BEHAVIOR INTERNALLY
         */
        
        Surface src(loadImage(textureRequest.inputSource->loadDataSource()));
        
        int srcWidth = src.getWidth();
        int srcHeight = src.getHeight();
        
        if (isOverSized(textureRequest, src.getSize()))
        {
            return TextureData(textureRequest, nullptr, 0, 0, srcWidth, srcHeight);
        }
        else
        {
            int dstWidth = nextPowerOfTwo(srcWidth);
            int dstHeight = nextPowerOfTwo(srcHeight);
            
            if ((srcWidth != dstWidth) || (srcHeight != dstHeight))
            {
                Surface dst(dstWidth, dstHeight, src.hasAlpha(), src.getChannelOrder());
                
                /*
                 * NO NEED TO CLEAR THE WHOLE SURFACE
                 */
                ip::fill(&dst, ColorA::zero(), Area(srcWidth + 1, 0, dstWidth, srcHeight));
                ip::fill(&dst, ColorA::zero(), Area(0, srcHeight + 1, srcWidth, dstHeight));
                ip::fill(&dst, ColorA::zero(), Area(srcWidth + 1, srcHeight + 1, dstWidth, dstHeight));
                
                dst.copyFrom(src, Area(0, 0, srcWidth, srcHeight), Vec2i::zero());
                
                /*
                 * DUPLICATING THE RIGHT AND BOTTOM EDGES:
                 * NECESSARY TO AVOID BORDER ARTIFACTS WHEN THE
                 * TEXTURE IS NOT DRAWN AT ITS ORIGINAL SCALE
                 */
                dst.copyFrom(src, Area(srcWidth - 1, 0, srcWidth, srcHeight), Vec2i(1, 0));
                dst.copyFrom(src, Area(0, srcHeight - 1, srcWidth, srcHeight), Vec2i(0, 1));
                
                return TextureData(textureRequest, dst, srcWidth / float(dstWidth), srcHeight / float(dstHeight));
            }
            else
            {
                return TextureData(textureRequest, src);
            }
        }
    }
    
    // ---
    
    Vec2i TextureHelper::getTextureSize(const TextureData &textureData)
    {
        if (!textureData.undefined())
        {
            switch (textureData.type)
            {
                case TextureData::TYPE_SURFACE:
                    return textureData.surface.getSize();
                    
                case TextureData::TYPE_IMAGE_SOURCE:
                    return Vec2i(textureData.imageSource->getWidth(), textureData.imageSource->getHeight());
                    
                case TextureData::TYPE_PVR:
                    return PVRHelper::getTextureSize(textureData.buffer);
                    
                case TextureData::TYPE_DATA:
                    return Vec2i(textureData.width, textureData.height);
            }
        }
        
        return Vec2i::zero();
    }
    
    size_t TextureHelper::getTextureMemoryUsage(const TextureData &textureData)
    {
        size_t memoryUsage = 0;
        
        if (!textureData.undefined())
        {
            switch (textureData.type)
            {
                case TextureData::TYPE_SURFACE:
                {
                    memoryUsage = textureData.surface.getHeight() * textureData.surface.getRowBytes(); // TODO: VERIFY
                    break;
                }
                    
                case TextureData::TYPE_IMAGE_SOURCE:
                {
                    auto size = Vec2i(textureData.imageSource->getWidth(), textureData.imageSource->getHeight());
                    auto channelOrder = textureData.imageSource->getChannelOrder();
                    auto dataType = textureData.imageSource->getDataType();
                    memoryUsage = size.x * size.y * ImageIo::channelOrderNumChannels(channelOrder) * ImageIo::dataTypeBytes(dataType);
                    break;
                }
                    
                case TextureData::TYPE_PVR:
                {
                    memoryUsage = PVRHelper::getTextureMemoryUsage(textureData.buffer); // TODO: VERIFY
                    break;
                }
                    
                case TextureData::TYPE_DATA:
                {
                    /*
                     * ASSUMING THAT THE GL-TYPE IS "GL_UNSIGNED_BYTE"
                     * AND HANDLING ONLY A LIMITED SET OF GL-INTERNAL-FORMATS...
                     */
                    
                    int bpp = 0;
                    
                    switch (textureData.glInternalFormat)
                    {
                        case GL_ALPHA:
                        case GL_LUMINANCE:
                            bpp = 1;
                            break;
                            
                        case GL_LUMINANCE_ALPHA:
                            bpp = 2;
                            break;

                        case GL_RGB:
                        case GL_RGBA:
                            bpp = 4;
                            break;
                    }
                    
                    memoryUsage = textureData.width * textureData.height * bpp;
                }
            }
            
            if (textureData.request.useMipmap)
            {
                memoryUsage *= 1.33f; // TODO: VERIFY
            }
        }
        
        return memoryUsage;
    }
}
