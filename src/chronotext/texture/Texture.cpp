#include "chronotext/texture/Texture.h"

using namespace std;
using namespace ci;

Texture::Texture(InputSourceRef inputSource, bool useMipmap, int flags, GLenum wrapS, GLenum wrapT)
:
inputSource(inputSource),
useMipmap(useMipmap),
flags(flags),
wrapS(wrapS),
wrapT(wrapT)
{
    init(TextureHelper::loadTexture(inputSource, useMipmap, flags, wrapS, wrapT));
}

Texture::~Texture()
{
    unload();
}

/*
 * ASSERTION: texture IS NULL
 */
void Texture::init(ci::gl::Texture *texture)
{
    target = texture;

    name = texture->getId();
    width = texture->getWidth();
    height = texture->getHeight();
    maxU = texture->getMaxU();
    maxV = texture->getMaxV();
}

void Texture::unload()
{
    if (target)
    {
        TextureHelper::deleteTexture(target);
        target = NULL;
    }
}

void Texture::reload()
{
    if (!target)
    {
        init(TextureHelper::loadTexture(inputSource, useMipmap, flags, wrapS, wrapT));
    }
}

int Texture::getId()
{
    return name;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, name);
}

void Texture::begin()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, name);
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
    const GLfloat vertices[] =
    {
        rect.x1, rect.y1,
        rect.x2, rect.y1,
        rect.x2, rect.y2,
        rect.x1, rect.y2
    };
    
    float tx1 = (rect.x1 - ox) / width;
    float ty1 = (rect.y1 - oy) / height;
    float tx2 = (rect.x2 - ox) / width;
    float ty2 = (rect.y2 - oy) / height;
    
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
