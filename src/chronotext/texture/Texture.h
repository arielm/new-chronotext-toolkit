/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/texture/TextureData.h"

namespace chronotext
{
    typedef std::shared_ptr<class Texture> TextureRef;
    
    class Texture
    {
    public:
        class Exception : public std::exception
        {
            std::string message;
            
        public:
            Exception(const std::string &what) throw() : message(what) {}
            ~Exception() throw() {}
            
            const char* what() const throw()
            {
                return message.c_str();
            }
        };
        
        TextureRequest request;
        
        Texture(InputSourceRef inputSource, bool useMipmap = false, int flags = TextureRequest::FLAGS_NONE);
        Texture(const TextureRequest &textureRequest);
        Texture(const TextureData &textureData);
        
        void discard();
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
        
    protected:
        ci::gl::TextureRef target;
        
        GLuint id;
        int width;
        int height;
        float maxU;
        float maxV;
        
        void setTarget(ci::gl::TextureRef texture);
    };
}

namespace chr = chronotext;
