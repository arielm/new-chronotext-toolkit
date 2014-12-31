/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/Context.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

#include "cinder/ip/Fill.h"

using namespace std;
using namespace ci;
using namespace context;

namespace chr
{
    MemoryInfo TextureHelper::memoryInfo[2];
    map<gl::Texture*, TextureHelper::MemoryProbe> TextureHelper::probes;

    // ---
    
    gl::TextureRef TextureHelper::loadTexture(InputSource::Ref inputSource, bool useMipmap, Texture::Request::Flags flags)
    {
        return loadTexture(Texture::Request(inputSource, useMipmap, flags));
    }
    
    gl::TextureRef TextureHelper::loadTexture(const Texture::Request &textureRequest)
    {
        auto textureData = fetchTextureData(textureRequest);
        
        if (textureData.type == Texture::Data::TYPE_UNDEFINED)
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
    
    Texture::Data TextureHelper::fetchTextureData(const Texture::Request &textureRequest)
    {
        if (TextureManager::PROBE_MEMORY)
        {
            memoryInfo[0] = getMemoryInfo();
        }
        
        if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.gz"))
        {
            if (textureRequest.inputSource->isFile())
            {
                return Texture::Data(textureRequest, PVRHelper::decompressGZ(textureRequest.inputSource->getFilePath()));
            }
            else
            {
                throw EXCEPTION(Texture, "PVR.GZ TEXTURES CAN ONLY BE LOADED FROM FILES");
            }
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.ccz"))
        {
            return Texture::Data(textureRequest, PVRHelper::decompressCCZ(textureRequest.inputSource->loadDataSource()));
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr"))
        {
            return Texture::Data(textureRequest, textureRequest.inputSource->loadDataSource()->getBuffer());
        }
        else
        {
            if (textureRequest.flags & Texture::Request::FLAGS_TRANSLUCENT)
            {
                return Texture::Data(fetchTranslucentTextureData(textureRequest));
            }
            else if (textureRequest.flags & Texture::Request::FLAGS_POT)
            {
                return Texture::Data(fetchPowerOfTwoTextureData(textureRequest));
            }
            else
            {
                return Texture::Data(textureRequest, loadImage(textureRequest.inputSource->loadDataSource()));
            }
        }
        
        return Texture::Data(textureRequest);
    }
    
    gl::TextureRef TextureHelper::uploadTextureData(const Texture::Data &textureData)
    {
        gl::TextureRef texture;
        
        if (textureData.type != Texture::Data::TYPE_UNDEFINED)
        {
            if (TextureManager::PROBE_MEMORY)
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
                case Texture::Data::TYPE_SURFACE:
                    texture = gl::Texture::create(textureData.surface, format);
                    texture->setCleanTexCoords(textureData.maxU, textureData.maxV);
                    break;
                    
                case Texture::Data::TYPE_IMAGE_SOURCE:
                    texture = gl::Texture::create(textureData.imageSource, format);
                    break;
                    
                case Texture::Data::TYPE_PVR:
                    texture = PVRHelper::loadTexture(textureData.buffer, format.hasMipmapping(), format.getWrapS(), format.getWrapT());
                    break;
                    
                case Texture::Data::TYPE_DATA:
                    format.setInternalFormat(textureData.glInternalFormat);
                    texture = gl::Texture::create(textureData.data.get(), textureData.glFormat, textureData.width, textureData.height, format);
                    break;
                    
                default:
                    assert(false); // UNREACHABLE
            }
            
            if (glGetError() == GL_OUT_OF_MEMORY)
            {
                throw EXCEPTION(Texture, "GL: OUT-OF-MEMORY");
            }
            else if (texture)
            {
                auto target = texture.get();
                
                texture->setDeallocator(&TextureHelper::textureDeallocator, target);

                auto memoryUsage = getTextureMemoryUsage(textureData);
                probes[target] = MemoryProbe({textureData.request, memoryUsage, memoryInfo[0], memoryInfo[1]});
            }
        }
        
        return texture;
    }
    
    // ---
    
    void TextureHelper::textureDeallocator(void *refcon)
    {
        auto target = reinterpret_cast<gl::Texture*>(refcon);
        
        if (TextureManager::PROBE_MEMORY)
        {
            probes[target].memoryInfo[2] = getMemoryInfo();
        }
    }
    
    bool TextureHelper::isOverSized(const Texture::Request &textureRequest, const Vec2i &size)
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
    
    Texture::Data TextureHelper::fetchTranslucentTextureData(const Texture::Request &textureRequest)
    {
        Surface surface(loadImage(textureRequest.inputSource->loadDataSource()));
        
        Channel8u &channel = surface.getChannel(0);
        int width = channel.getWidth();
        int height = channel.getHeight();
        
        if (isOverSized(textureRequest, channel.getSize()))
        {
            return Texture::Data(textureRequest, nullptr, 0, 0, width, height);
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
            
            return Texture::Data(textureRequest, data, GL_ALPHA, GL_ALPHA, width, height);
        }
    }
    
    Texture::Data TextureHelper::fetchPowerOfTwoTextureData(const Texture::Request &textureRequest)
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
            return Texture::Data(textureRequest, nullptr, 0, 0, srcWidth, srcHeight);
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
                
                return Texture::Data(textureRequest, dst, srcWidth / float(dstWidth), srcHeight / float(dstHeight));
            }
            else
            {
                return Texture::Data(textureRequest, src);
            }
        }
    }
    
    // ---
    
    Vec2i TextureHelper::getTextureSize(const Texture::Data &textureData)
    {
        switch (textureData.type)
        {
            case Texture::Data::TYPE_SURFACE:
                return textureData.surface.getSize();
                
            case Texture::Data::TYPE_IMAGE_SOURCE:
                return Vec2i(textureData.imageSource->getWidth(), textureData.imageSource->getHeight());
                
            case Texture::Data::TYPE_PVR:
                return PVRHelper::getTextureSize(textureData.buffer);
                
            case Texture::Data::TYPE_DATA:
                return Vec2i(textureData.width, textureData.height);
                
            default:
                return Vec2i::zero();
        }
    }
    
    int64_t TextureHelper::getTextureMemoryUsage(const Texture::Data &textureData)
    {
        int64_t memoryUsage = 0;
        bool rgb = false;
        
        switch (textureData.type)
        {
            case Texture::Data::TYPE_SURFACE:
            {
                auto size = getTextureSize(textureData);
                auto rowBytes = textureData.surface.getRowBytes();
                auto channelOrderNumChannels = ImageIo::channelOrderNumChannels(ImageIo::ChannelOrder(textureData.surface.getChannelOrder().getImageIoChannelOrder()));
                
                memoryUsage = size.y * rowBytes;
                rgb = (channelOrderNumChannels == 3);
                break;
            }
                
            case Texture::Data::TYPE_IMAGE_SOURCE:
            {
                auto size = getTextureSize(textureData);
                auto dataTypeBytes = ImageIo::dataTypeBytes(textureData.imageSource->getDataType());
                auto channelOrderNumChannels = ImageIo::channelOrderNumChannels(textureData.imageSource->getChannelOrder());
                
                memoryUsage = size.x * size.y * dataTypeBytes * channelOrderNumChannels;
                rgb = (channelOrderNumChannels == 3);
                break;
            }
                
            case Texture::Data::TYPE_PVR:
            {
                memoryUsage = PVRHelper::getTextureMemoryUsage(textureData.buffer); // TODO: VERIFY
                break;
            }
                
            case Texture::Data::TYPE_DATA:
            {
                /*
                 * CURRENT LIMITATIONS:
                 *
                 * - ASSUMING THAT THE GL-TYPE IS "GL_UNSIGNED_BYTE"
                 * - HANDLING ONLY A LIMITED SET OF GL-INTERNAL-FORMATS
                 */
                
                int bpp = 0;
                
                switch (textureData.glInternalFormat)
                {
                    case GL_ALPHA:
                    case GL_LUMINANCE:
                        bpp = 8;
                        break;
                        
                    case GL_LUMINANCE_ALPHA:
                        bpp = 16;
                        break;
                        
                    case GL_RGB:
                        bpp = 24;
                        break;
                        
                    case GL_RGBA:
                        bpp = 32;
                        break;
                }
                
                memoryUsage = (textureData.width * textureData.height * bpp) >> 3;
                rgb = (bpp == 24);
                break;
            }
                
            default:
                return 0;
        }
        
        if (rgb && true)
        {
            memoryUsage *= 4;
            memoryUsage /= 3;
        }
        
        if (textureData.request.useMipmap)
        {
            memoryUsage *= 1.33;
        }
        
        return memoryUsage;
    }
}
