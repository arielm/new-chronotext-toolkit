/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"

#include "cinder/gl/Texture.h"

namespace chr
{
    class TextureData;

    class Texture
    {
    public:
        static bool VERBOSE;

        typedef std::shared_ptr<Texture> Ref;

        struct Request
        {
            enum Flags
            {
                FLAGS_NONE = 0,
                FLAGS_TRANSLUCENT = 1,
                FLAGS_POT = 2
            };
            
            InputSource::Ref inputSource;
            bool useMipmap;
            Flags flags;
            GLenum wrapS;
            GLenum wrapT;
            ci::Vec2i maxSize;
            
            Request()
            :
            useMipmap(false),
            flags(FLAGS_NONE),
            wrapS(0),
            wrapT(0)
            {}
            
            Request(InputSource::Ref inputSource, bool useMipmap = false, Flags flags = FLAGS_NONE)
            :
            inputSource(inputSource),
            useMipmap(useMipmap),
            flags(flags),
            wrapS(GL_CLAMP_TO_EDGE),
            wrapT(GL_CLAMP_TO_EDGE),
            maxSize(0, 0)
            {}
            
            ci::gl::Texture::Format getFormat() const
            {
                ci::gl::Texture::Format format;
                format.setWrap(wrapS, wrapT);
                
                if (useMipmap)
                {
                    format.enableMipmapping(true);
                    format.setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
                }
                
                return format;
            }
            
            Request& setWrap(GLenum s, GLenum t)
            {
                wrapS = s;
                wrapT = t;
                return *this;
            }
            
            Request& setMaxSize(const ci::Vec2i &size)
            {
                maxSize = size;
                return *this;
            }
            
            bool operator<(const Request &rhs) const
            {
                if (std::tie(useMipmap, flags, wrapS, wrapT) == std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT))
                {
                    return (inputSource->getURI() < rhs.inputSource->getURI());
                }
                else
                {
                    return std::tie(useMipmap, flags, wrapS, wrapT) < std::tie(rhs.useMipmap, rhs.flags, rhs.wrapS, rhs.wrapT);
                }
            }
        };
        
        // ---
        
        Request request;
        uint32_t glId;
        
        Texture(InputSource::Ref inputSource, bool useMipmap = false, Request::Flags flags = Request::FLAGS_NONE);
        Texture(const Request &request);
        Texture(const TextureData &data);
        
        ~Texture();
        
        void discard();
        bool reload();
        
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
        
        int width;
        int height;
        float maxU;
        float maxV;
        
        void setTarget(ci::gl::TextureRef target);
        void resetTarget();
    };
}
