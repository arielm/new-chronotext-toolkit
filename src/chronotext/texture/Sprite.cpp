#include "chronotext/texture/Sprite.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    Sprite::Sprite(TextureRef texture)
    :
    texture(texture),
    ox(0),
    oy(0),
    rotated(false),
    tx1(0),
    ty1(0),
    tx2(texture->getMaxU()),
    ty2(texture->getMaxV())
    {
        w = ow = texture->getWidth() * texture->getMaxU();
        h = oh = texture->getHeight() * texture->getMaxV();
    }
    
    Sprite::Sprite(TextureRef texture, float w, float h, float ox, float oy, float ow, float oh, bool rotated, float tx1, float ty1, float tx2, float ty2)
    :
    texture(texture),
    w(w),
    h(h),
    ox(ox),
    oy(oy),
    ow(ow),
    oh(oh),
    rotated(rotated),
    tx1(tx1),
    ty1(ty1),
    tx2(tx2),
    ty2(ty2)
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
        float vertices[4][2];
        float coords[4][2];
        
        float x1 = -rx + ox;
        float y1 = -ry + oy;
        
        float x2 = x1 + w;
        float y2 = y1 + h;
        
        if (rotated)
        {
            vertices[0][0] = y1; vertices[0][1] = x2;
            vertices[1][0] = y2; vertices[1][1] = x2;
            vertices[2][0] = y2; vertices[2][1] = x1;
            vertices[3][0] = y1; vertices[3][1] = x1;
            
            coords[0][0] = tx1; coords[0][1] = ty1;
            coords[1][0] = tx1; coords[1][1] = ty2;
            coords[2][0] = tx2; coords[2][1] = ty2;
            coords[3][0] = tx2; coords[3][1] = ty1;
        }
        else
        {
            vertices[0][0] = x1; vertices[0][1] = y1;
            vertices[1][0] = x2; vertices[1][1] = y1;
            vertices[2][0] = x2; vertices[2][1] = y2;
            vertices[3][0] = x1; vertices[3][1] = y2;
            
            coords[0][0] = tx1; coords[0][1] = ty1;
            coords[1][0] = tx2; coords[1][1] = ty1;
            coords[2][0] = tx2; coords[2][1] = ty2;
            coords[3][0] = tx1; coords[3][1] = ty2;
        }
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);    
    }
}
