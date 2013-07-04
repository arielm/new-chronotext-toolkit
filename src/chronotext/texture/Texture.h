#pragma once

#include "chronotext/texture/TextureData.h"

typedef std::shared_ptr<class Texture> TextureRef;

class Texture
{
    ci::gl::TextureRef target;

    GLuint name;
    int width;
    int height;
    float maxU;
    float maxV;
    
    void setTarget(ci::gl::TextureRef texture);

public:
    TextureRequest request;

    Texture(InputSourceRef inputSource, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE, GLenum wrapS = GL_CLAMP_TO_EDGE, GLenum wrapT = GL_CLAMP_TO_EDGE);
    Texture(const TextureRequest &textureRequest);
    Texture(const TextureData &textureData);
    
    void unload();
    void reload();
    
    TextureData fetchTextureData();
    void uploadTextureData(const TextureData &textureData);

    int getId() const;
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
