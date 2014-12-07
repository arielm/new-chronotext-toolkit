/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/Sprite.h"

using namespace std;
using namespace ci;

namespace chr
{
    Sprite::Sprite()
    {}
    
    Sprite::Sprite(Texture::Ref texture)
    :
    texture(texture),
    ox(0),
    oy(0),
    rotated(false),
    u1(0),
    v1(0),
    u2(texture->getMaxU()),
    v2(texture->getMaxV())
    {
        w = ow = texture->getCleanWidth();
        h = oh = texture->getCleanHeight();
    }
    
    Sprite::Sprite(Texture::Ref texture, float w, float h, float ox, float oy, float ow, float oh, bool rotated, float u1, float v1, float u2, float v2)
    :
    texture(texture),
    w(w),
    h(h),
    ox(ox),
    oy(oy),
    ow(ow),
    oh(oh),
    rotated(rotated),
    u1(u1),
    v1(v1),
    u2(u2),
    v2(v2)
    {}
    
    Vec2f Sprite::getSize() const
    {
        return Vec2f(ow, oh);
    }
    
    float Sprite::getWidth() const
    {
        return ow;
    }
    
    float Sprite::getHeight() const
    {
        return oh;
    }
    
    void Sprite::beginTexture()
    {
        texture->begin();
    }
    
    void Sprite::endTexture()
    {
        texture->end();
    }
    
    void Sprite::drawFromCenter()
    {
        draw(ow * 0.5f, oh * 0.5f);
    }
    
    void Sprite::draw(float rx, float ry)
    {
        float x1, y1;
        
        if (rotated)
        {
            x1 = -ry + oy;
            y1 = -rx + ox;
        }
        else
        {
            x1 = -rx + ox;
            y1 = -ry + oy;
        }
        
        float x2 = x1 + w;
        float y2 = y1 + h;
        
        GLfloat vertices[4][2];
        GLfloat coords[4][2];
        
        if (rotated)
        {
            vertices[0][0] = y1; vertices[0][1] = x2;
            vertices[1][0] = y2; vertices[1][1] = x2;
            vertices[2][0] = y2; vertices[2][1] = x1;
            vertices[3][0] = y1; vertices[3][1] = x1;
            
            coords[0][0] = u1; coords[0][1] = v1;
            coords[1][0] = u1; coords[1][1] = v2;
            coords[2][0] = u2; coords[2][1] = v2;
            coords[3][0] = u2; coords[3][1] = v1;
        }
        else
        {
            vertices[0][0] = x1; vertices[0][1] = y1;
            vertices[1][0] = x2; vertices[1][1] = y1;
            vertices[2][0] = x2; vertices[2][1] = y2;
            vertices[3][0] = x1; vertices[3][1] = y2;
            
            coords[0][0] = u1; coords[0][1] = v1;
            coords[1][0] = u2; coords[1][1] = v1;
            coords[2][0] = u2; coords[2][1] = v2;
            coords[3][0] = u1; coords[3][1] = v2;
        }
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    
    }
}
