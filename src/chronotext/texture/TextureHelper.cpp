#include "chronotext/texture/TextureHelper.h"
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

gl::Texture* TextureHelper::loadTexture(const string &resourceName, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
{
    return loadTexture(InputSource::getResource(resourceName), useMipmap, flags, wrapS, wrapT);
}

gl::Texture* TextureHelper::loadTexture(InputSourceRef inputSource, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
{
    gl::Texture *texture = uploadTexture(getTextureData(inputSource, useMipmap, flags, wrapS, wrapT));
    
    if (texture)
    {
        LOGD << "TEXTURE LOADED: " << inputSource->getFilePathHint() << " | " << texture->getId() << " | " << texture->getWidth() << "x" << texture->getHeight() << endl;
    }
    
    return texture;
}

TextureData TextureHelper::getTextureData(InputSourceRef inputSource, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
{
    TextureData textureData;

    gl::Texture::Format format;
    format.setWrap(wrapS, wrapT);
    
    if (useMipmap)
    {
        format.enableMipmapping(true);
        format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    }
    
    if (inputSource->getFilePathHint().rfind(".pvr.gz") != string::npos)
    {
        if (inputSource->isFile())
        {
            textureData = TextureData(inputSource, format, PVRHelper::decompressPVRGZ(inputSource->getFilePath()));
        }
        else
        {
            throw runtime_error("PVR.GZ TEXTURES CAN ONLY BE LOADED FROM FILES");
        }
    }
    else if (inputSource->getFilePathHint().rfind(".pvr.ccz") != string::npos)
    {
        textureData = TextureData(inputSource, format, PVRHelper::decompressPVRCCZ(inputSource->loadDataSource()));
    }
    else if (inputSource->getFilePathHint().rfind(".pvr") != string::npos)
    {
        textureData = TextureData(inputSource, format, inputSource->loadDataSource()->getBuffer());
    }
    else
    {
        if (flags & FLAGS_TRANSLUCENT)
        {
            textureData = TextureData(getTranslucentTextureData(inputSource, format));
        }
        else if (flags & FLAGS_POT)
        {
            textureData = TextureData(getPowerOfTwoTextureData(inputSource, format));
        }
        else
        {
            textureData = TextureData(inputSource, format, loadImage(inputSource->loadDataSource()));
        }
    }

    return textureData;
}

gl::Texture* TextureHelper::uploadTexture(const TextureData &textureData)
{
    if (!textureData.undefined())
    {
        switch (textureData.type)
        {
            case TextureData::TYPE_SURFACE:
            {
                gl::Texture *texture = new gl::Texture(textureData.surface, textureData.format);
                texture->setCleanTexCoords(textureData.maxU, textureData.maxV);
                return texture;
            }
                
            case TextureData::TYPE_IMAGE_SOURCE:
            {
                return new gl::Texture(textureData.imageSource, textureData.format);
            }
                
            case TextureData::TYPE_PVR:
            {
                return PVRHelper::getPVRTexture(textureData.buffer, textureData.format.hasMipmapping(), textureData.format.getWrapS(), textureData.format.getWrapT());
            }
                
            case TextureData::TYPE_DATA:
            {
                return new gl::Texture(textureData.data.get(), textureData.dataFormat, textureData.width, textureData.height, textureData.format);
            }
        }
    }
    
    return NULL;
}

void TextureHelper::unloadTexture(ci::gl::Texture *texture)
{
    LOGD << "TEXTURE UNLOADED: " << texture->getId() << endl;
    delete texture;
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
    
    const GLfloat vertices[] =
    {
        x1, y1,
        x2, y1,
        x2, y2,
        x1, y2
    };
    
    const GLfloat coords[] =
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
    const GLfloat vertices[] =
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
    
	const GLfloat coords[] =
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

/*
 * BASED ON CODE FROM cinder/gl/Texture.cpp
 */
TextureData TextureHelper::getTranslucentTextureData(InputSourceRef inputSource, gl::Texture::Format &format)
{
    Surface surface(loadImage(inputSource->loadDataSource()));
    
    Channel8u channel = surface.getChannel(0);
    shared_ptr<uint8_t> data;
    
    GLenum dataFormat = GL_ALPHA;
    format.setInternalFormat(GL_ALPHA);
    
    // if the data is not already contiguous, we'll need to create a block of memory that is
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
    
    return TextureData(inputSource, format, data, dataFormat, channel.getWidth(), channel.getHeight());
}

TextureData TextureHelper::getPowerOfTwoTextureData(InputSourceRef inputSource, gl::Texture::Format &format)
{
    Surface src(loadImage(inputSource->loadDataSource()));
    
    int srcWidth = src.getWidth();
    int srcHeight = src.getHeight();
    
    int dstWidth = nextPOT(srcWidth);
    int dstHeight = nextPOT(srcHeight);
    
    if ((srcWidth != dstWidth) || (srcHeight != dstHeight))
    {
        Surface dst(dstWidth, dstHeight, src.hasAlpha(), src.getChannelOrder());
        
        ip::fill(&dst, ColorA::zero());
        dst.copyFrom(src, Area(0, 0, srcWidth, srcHeight), Vec2i::zero());
        
        return TextureData(inputSource, format, dst, srcWidth / float(dstWidth), srcHeight / float(dstHeight));
    }
    else
    {
        return TextureData(inputSource, format, src);
    }
}
