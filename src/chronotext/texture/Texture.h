#pragma once

#include "chronotext/texture/TextureHelper.h"

class Texture
{
    GLuint name;
    int width;
    int height;
    float maxU;
    float maxV;
    
    ci::gl::Texture *target;
    
    void init(ci::gl::Texture *texture);

public:
    InputSourceRef inputSource;
    bool useMipmap;
    int flags;
    GLenum wrapS;
    GLenum wrapT;

    Texture(InputSourceRef inputSource, bool useMipmap = false, int flags = TextureHelper::FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    ~Texture();
    
    void unload();
    void reload();

    int getId();
    void bind();
    
    void begin();
    void end();
    
    void drawFromCenter();
    void draw(float rx = 0, float ry = 0);
    void drawInRect(const ci::Rectf &rect, float ox = 0, float oy = 0);
    
    int getWidth() const;
    int getHeight() const;
    ci::Vec2i getSize() const;
    
    float getMaxU() const;
    float getMaxV() const;
};
