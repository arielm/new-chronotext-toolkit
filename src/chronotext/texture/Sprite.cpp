#include "chronotext/texture/Sprite.h"

using namespace std;
using namespace ci;

Vec2f Sprite::getSize() const
{
    return Vec2f(ow - ox, oh - oy);
}

float Sprite::getWidth() const
{
    return ow - ox;
}

float Sprite::getHeight() const
{
    return oh - oy;
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
    float x1;
    float y1;
    
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
