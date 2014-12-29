/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/InputSource.h"
#include "chronotext/texture/PVRHelper.h"

#include "cinder/ImageIo.h"

namespace chr
{
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
            
            int tag;
            InputSource::Ref inputSource;
            
            bool useMipmap;
            Flags flags;
            GLenum wrapS;
            GLenum wrapT;
            ci::Vec2i maxSize;
            
            Request() = default;
            
            Request(InputSource::Ref inputSource, bool useMipmap = false, Flags flags = FLAGS_NONE)
            :
            tag(0),
            inputSource(inputSource),
            useMipmap(useMipmap),
            flags(flags),
            wrapS(GL_CLAMP_TO_EDGE),
            wrapT(GL_CLAMP_TO_EDGE),
            maxSize(0, 0)
            {}
            
            Request& setTag(int tag)
            {
                Request::tag = tag;
                return *this;
            }
            
            Request& setWrap(GLenum s, GLenum t)
            {
                wrapS = s;
                wrapT = t;
                return *this;
            }
            
            Request& setMaxSize(int width, int height)
            {
                maxSize.x = width;
                maxSize.y = height;
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
        };
        
        struct Data
        {
            enum Type
            {
                TYPE_UNDEFINED,
                TYPE_SURFACE,
                TYPE_IMAGE_SOURCE,
                TYPE_PVR,
                TYPE_DATA
            };
            
            Type type;
            Request request;
            
            /*
             * NO SPECIAL-TREATMENT IS NECESSARY FOR surface AND buffer IN ORDER TO AVOID EXTRA DATA-COPYING
             * SINCE ci::Surface AND ci::Buffer EMULATE shared_ptr INTERNALLY
             */
            ci::Surface surface;
            ci::ImageSourceRef imageSource;
            ci::Buffer buffer;
            std::shared_ptr<uint8_t> data;
            
            float maxU;
            float maxV;
            
            GLenum glInternalFormat;
            GLenum glFormat;
            int width;
            int height;
            
            Data(const Request &request)
            :
            type(TYPE_UNDEFINED),
            request(request)
            {}
            
            Data(const Request &request, const ci::Surface &surface, float maxU = 1, float maxV = 1)
            :
            type(TYPE_SURFACE),
            request(request),
            surface(surface),
            maxU(maxU),
            maxV(maxV)
            {}
            
            Data(const Request &request, ci::ImageSourceRef imageSource)
            :
            type(TYPE_IMAGE_SOURCE),
            request(request),
            imageSource(imageSource)
            {}
            
            Data(const Request &request, const ci::Buffer &buffer)
            :
            type(TYPE_PVR),
            request(request),
            buffer(buffer)
            {}
            
            Data(const Request &request, std::shared_ptr<uint8_t> data, GLenum glInternalFormat, GLenum glFormat, int width, int height)
            :
            type(TYPE_DATA),
            request(request),
            data(data),
            glInternalFormat(glInternalFormat),
            glFormat(glFormat),
            width(width),
            height(height)
            {}
        };
        
        // ---
        
        Request request;
        uint32_t glId;
        
        Texture(InputSource::Ref inputSource, bool useMipmap = false, Request::Flags flags = Request::FLAGS_NONE);
        Texture(const Request &request);
        Texture(const Data &data);
        
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
        
        size_t getMemoryUsage() const;
        
        void bind();
        void begin();
        void end();
        
        void drawFromCenter();
        void draw(float rx = 0, float ry = 0);
        void drawInRect(const ci::Rectf &rect, float ox = 0, float oy = 0);
        
    protected:
        ci::gl::TextureRef target;
        size_t memoryUsage;
        
        int width;
        int height;
        float maxU;
        float maxV;
        
        void setTarget(ci::gl::TextureRef target);
        void resetTarget();
    };
}
