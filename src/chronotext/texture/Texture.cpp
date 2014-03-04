/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/Texture.h"
#include "chronotext/texture/TextureHelper.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Texture::Texture(InputSourceRef inputSource, bool useMipmap, TextureRequest::Flags flags)
    :
    request(TextureRequest(inputSource, useMipmap, flags))
    {
        setTarget(TextureHelper::loadTexture(request));
    }
    
    Texture::Texture(const TextureRequest &textureRequest)
    :
    request(textureRequest)
    {
        setTarget(TextureHelper::loadTexture(request));
    }
    
    Texture::Texture(const TextureData &textureData)
    :
    request(textureData.request)
    {
        setTarget(TextureHelper::uploadTextureData(textureData));
    }
    
    void Texture::discard()
    {
        if (target)
        {
            target.reset();
        }
    }
    
    void Texture::reload()
    {
        if (!target)
        {
            setTarget(TextureHelper::loadTexture(request));
        }
    }
    
    TextureData Texture::fetchTextureData()
    {
        return TextureHelper::fetchTextureData(request);
    }
    
    void Texture::uploadTextureData(const TextureData &textureData)
    {
        request = textureData.request;
        setTarget(TextureHelper::uploadTextureData(textureData));
    }
    
    int Texture::getId() const
    {
        return id;
    }
    
    void Texture::bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }
    
    void Texture::begin()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, id);
    }
    
    void Texture::end()
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    void Texture::drawFromCenter()
    {
        draw(width * maxU * 0.5f, height * maxV * 0.5f);
    }
    
    void Texture::draw(float rx, float ry)
    {
        float x1 = -rx;
        float y1 = -ry;
        
        float x2 = x1 + width * maxU;
        float y2 = y1 + height * maxV;
        
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
            maxU, 0,
            maxU, maxV,
            0, maxV
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    void Texture::drawInRect(const Rectf &rect, float ox, float oy)
    {
        const float vertices[] =
        {
            rect.x1, rect.y1,
            rect.x2, rect.y1,
            rect.x2, rect.y2,
            rect.x1, rect.y2
        };
        
        float u1 = (rect.x1 - ox) / width;
        float v1 = (rect.y1 - oy) / height;
        float u2 = (rect.x2 - ox) / width;
        float v2 = (rect.y2 - oy) / height;
        
        const float coords[] =
        {
            u1, v1,
            u2, v1,
            u2, v2,
            u1, v2
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    int Texture::getWidth() const
    {
        return width;
    }
    
    int Texture::getHeight() const
    {
        return height;
    }
    
    Vec2i Texture::getSize() const
    {
        return Vec2i(width, height);
    }
    
    float Texture::getMaxU() const
    {
        return maxU;
    }
    
    float Texture::getMaxV() const
    {
        return maxV;
    }
    
    void Texture::setTarget(ci::gl::TextureRef texture)
    {
        target = texture;
        
        id = texture->getId();
        width = texture->getWidth();
        height = texture->getHeight();
        maxU = texture->getMaxU();
        maxV = texture->getMaxV();
    }
}
