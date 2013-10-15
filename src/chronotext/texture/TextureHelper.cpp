/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/Texture.h"
#include "chronotext/texture/PVRHelper.h"
#include "chronotext/utils/Utils.h"

#include "cinder/ImageIo.h"
#include "cinder/ip/Fill.h"

using namespace std;
using namespace ci;

static int nextPOT(int val)
{
    int ret = 1;
    while (ret < val)
    {
        ret <<= 1;
    }
    return ret;
}

namespace chronotext
{
    gl::TextureRef TextureHelper::loadTexture(const string &resourceName, bool useMipmap, int flags)
    {
        return loadTexture(InputSource::getResource(resourceName), useMipmap, flags);
    }
    
    gl::TextureRef TextureHelper::loadTexture(InputSourceRef inputSource, bool useMipmap, int flags)
    {
        return loadTexture(TextureRequest(inputSource, useMipmap, flags));
    }
    
    gl::TextureRef TextureHelper::loadTexture(const TextureRequest &textureRequest)
    {
        TextureData textureData = fetchTextureData(textureRequest);
        
        if (textureData.undefined())
        {
            throw Texture::Exception("TEXTURE IS UNDEFINED");
        }
        else
        {
            const Vec2i size = textureData.getSize();
            
            if ((size.x > textureRequest.maxSize.x) || (size.y > textureRequest.maxSize.y))
            {
                throw Texture::Exception("TEXTURE IS OVER-SIZED");
            }
        }

        return uploadTextureData(textureData);
    }
    
    TextureData TextureHelper::fetchTextureData(const TextureRequest &textureRequest)
    {
        TextureData textureData;
        
        if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.gz"))
        {
            if (textureRequest.inputSource->isFile())
            {
                textureData = TextureData(textureRequest, PVRHelper::decompressPVRGZ(textureRequest.inputSource->getFilePath()));
            }
            else
            {
                throw Texture::Exception("PVR.GZ TEXTURES CAN ONLY BE LOADED FROM FILES");
            }
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr.ccz"))
        {
            textureData = TextureData(textureRequest, PVRHelper::decompressPVRCCZ(textureRequest.inputSource->loadDataSource()));
        }
        else if (boost::ends_with(textureRequest.inputSource->getFilePathHint(), ".pvr"))
        {
            textureData = TextureData(textureRequest, textureRequest.inputSource->loadDataSource()->getBuffer());
        }
        else
        {
            if (textureRequest.flags & TextureRequest::FLAGS_TRANSLUCENT)
            {
                textureData = TextureData(fetchTranslucentTextureData(textureRequest));
            }
            else if (textureRequest.flags & TextureRequest::FLAGS_POT)
            {
                textureData = TextureData(fetchPowerOfTwoTextureData(textureRequest));
            }
            else
            {
                textureData = TextureData(textureRequest, loadImage(textureRequest.inputSource->loadDataSource()));
            }
        }
        
        return textureData;
    }
    
    gl::TextureRef TextureHelper::uploadTextureData(const TextureData &textureData)
    {
        gl::TextureRef texture;
        
        if (!textureData.undefined())
        {
            gl::Texture::Format format = textureData.request.getFormat();
            
            /*
             * NECESSARY IN ORDER TO CLEANUP EVENTUAL ERRORS
             */
            while(glGetError() != GL_NO_ERROR)
            {}
            
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
                    texture = PVRHelper::getPVRTexture(textureData.buffer, format.hasMipmapping(), format.getWrapS(), format.getWrapT());
                    break;
                    
                case TextureData::TYPE_DATA:
                    format.setInternalFormat(textureData.glInternalFormat);
                    texture = gl::Texture::create(textureData.data.get(), textureData.glFormat, textureData.width, textureData.height, format);
                    break;
            }
            
            if (glGetError() == GL_OUT_OF_MEMORY)
            {
                throw ci::Exception();
            }
            else if (texture)
            {
                texture->setDeallocator(&TextureHelper::textureDeallocator, texture.get());
                
                LOGD <<
                "TEXTURE UPLOADED: " <<
                textureData.request.inputSource->getFilePathHint() << " | " <<
                texture->getId() << " | " <<
                texture->getWidth() << "x" << texture->getHeight() <<
                endl;
            }
        }
        
        return texture;
    }
    
    void TextureHelper::bindTexture(gl::Texture *texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture->getId());
    }
    
    void TextureHelper::beginTexture(gl::Texture *texture)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, texture->getId());
    }
    
    void TextureHelper::endTexture()
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    /*
     * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
     */
    void TextureHelper::drawTextureFromCenter(gl::Texture *texture)
    {
        drawTexture(texture, texture->getWidth() * texture->getMaxU() * 0.5f, texture->getHeight() * texture->getMaxV() * 0.5f);
    }
    
    /*
     * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
     */
    void TextureHelper::drawTexture(gl::Texture *texture, float rx, float ry)
    {
        float tx = texture->getMaxU();
        float ty = texture->getMaxV();
        
        float x1 = -rx;
        float y1 = -ry;
        
        float x2 = x1 + texture->getWidth() * tx;
        float y2 = y1 + texture->getHeight() * ty;
        
        const float vertices[] =
        {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };
        
        const float coords[] =
        {
            0, 0,
            tx, 0,
            tx, ty,
            0, ty
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    /*
     * XXX: ONLY WORKS FOR "TRUE" POWER-OF-TWO TEXTURES
     */
    void TextureHelper::drawTextureInRect(gl::Texture *texture, const Rectf &rect, float ox, float oy)
    {
        const float vertices[] =
        {
            rect.x1, rect.y1,
            rect.x2, rect.y1,
            rect.x2, rect.y2,
            rect.x1, rect.y2
        };
        
        float tx1 = (rect.x1 - ox) / texture->getWidth();
        float ty1 = (rect.y1 - oy) / texture->getHeight();
        float tx2 = (rect.x2 - ox) / texture->getWidth();
        float ty2 = (rect.y2 - oy) / texture->getHeight();
        
        const float coords[] =
        {
            tx1, ty1,
            tx2, ty1,
            tx2, ty2,
            tx1, ty2
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    void TextureHelper::textureDeallocator(void *refcon)
    {
        gl::Texture *texture = reinterpret_cast<gl::Texture*>(refcon);
        
        LOGD <<
        "TEXTURE UNLOADED: " <<
        texture->getId() <<
        endl;
    }
    
    /*
     * BASED ON https://github.com/cinder/Cinder/blob/v0.8.5/src/cinder/gl/Texture.cpp#L478-490
     */
    TextureData TextureHelper::fetchTranslucentTextureData(const TextureRequest &textureRequest)
    {
        Surface surface(loadImage(textureRequest.inputSource->loadDataSource()));
        
        Channel8u channel = surface.getChannel(0);
        shared_ptr<uint8_t> data;
        
        if ((channel.getIncrement() != 1 ) || (channel.getRowBytes() != channel.getWidth() * sizeof(uint8_t)))
        {
            data = shared_ptr<uint8_t>(new uint8_t[channel.getWidth() * channel.getHeight()], checked_array_deleter<uint8_t>());
            uint8_t *dest = data.get();
            const int8_t inc = channel.getIncrement();
            const int32_t width = channel.getWidth();
            
            for (int y = 0; y < channel.getHeight(); ++y)
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
        
        return TextureData(textureRequest, data, GL_ALPHA, GL_ALPHA, channel.getWidth(), channel.getHeight());
    }
    
    TextureData TextureHelper::fetchPowerOfTwoTextureData(const TextureRequest &textureRequest)
    {
        Surface src(loadImage(textureRequest.inputSource->loadDataSource()));
        
        int srcWidth = src.getWidth();
        int srcHeight = src.getHeight();
        
        int dstWidth = nextPOT(srcWidth);
        int dstHeight = nextPOT(srcHeight);
        
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
