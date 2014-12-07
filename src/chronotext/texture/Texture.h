/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/Exception.h"
#include "chronotext/texture/TextureData.h"

namespace chr
{
    class Texture
    {
    public:
        static bool VERBOSE;

        typedef std::shared_ptr<class Texture> Ref;

        TextureRequest request;
        
        Texture(InputSourceRef inputSource, bool useMipmap = false, TextureRequest::Flags flags = TextureRequest::FLAGS_NONE);
        Texture(const TextureRequest &textureRequest);
        Texture(const TextureData &textureData);
        
        ~Texture();
        
        void discard();
        void reload();
        
        TextureData fetchTextureData();
        void uploadTextureData(const TextureData &textureData);
        
        int getId() const;
        
        int getWidth() const;
        int getHeight() const;
        ci::Vec2i getSize() const;

        int getCleanWidth() const;
        int getCleanHeight() const;
        ci::Vec2i getCleanSize() const;

        float getMaxU() const;
        float getMaxV() const;
        ci::Vec2f getMaxUV() const;
        
        void bind();
        void begin();
        void end();
        
        void drawFromCenter();
        void draw(float rx = 0, float ry = 0);
        void drawInRect(const ci::Rectf &rect, float ox = 0, float oy = 0);
        
    protected:
        ci::gl::TextureRef target;
        
        uint32_t textureId;
        int width;
        int height;
        float maxU;
        float maxV;
        
        void setTarget(ci::gl::TextureRef texture);
        void resetTarget();
    };
}
