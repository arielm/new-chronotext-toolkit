/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/texture/Texture.h"
#include "chronotext/texture/TextureHelper.h"
#include "chronotext/texture/TextureManager.h"
#include "chronotext/Context.h"

using namespace std;
using namespace ci;
using namespace chr::utils;

namespace chr
{
    Texture::Texture(const Request &request, ci::gl::Texture *target, int uniqueId)
    :
    request(request),
    uniqueId(uniqueId),
    target(nullptr)
    {
        setTarget(target);
    }

    Texture::~Texture()
    {
        resetTarget();
    }
    
    void Texture::discard()
    {
        resetTarget();
    }
    
    bool Texture::reload()
    {
        if (!target)
        {
            setTarget(TextureHelper::loadTarget(request));
        }
        
        return bool(target);
    }
    
    int64_t Texture::getMemoryUsage() const
    {
        return memoryUsage;
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
    
    int Texture::getCleanWidth() const
    {
        return width * maxU;
    }
    
    int Texture::getCleanHeight() const
    {
        return height * maxV;
    }
    
    Vec2i Texture::getCleanSize() const
    {
        return Vec2i(width * maxU, height * maxV);
    }
    
    float Texture::getMaxU() const
    {
        return maxU;
    }
    
    float Texture::getMaxV() const
    {
        return maxV;
    }
    
    Vec2f Texture::getMaxUV() const
    {
        return Vec2f(maxU, maxV);
    }
    
    void Texture::setTarget(ci::gl::Texture* target)
    {
        if (!target)
        {
            resetTarget();
        }
        else if (!Texture::target)
        {
            auto it = TextureHelper::records.find(target);
            
            if (it == TextureHelper::records.end())
            {
                throw EXCEPTION(Texture, "UNREGISTERED TARGET");
            }
            
            const auto &record = it->second;
            
            // ---
            
            Texture::target = target;
            memoryUsage = record.memoryUsage;
            
            glId = target->getId();
            width = target->getWidth();
            height = target->getHeight();
            maxU = target->getMaxU();
            maxV = target->getMaxV();
            
            // ---
            
            stringstream memoryStats;
            
            if (TextureManager::PROBE_MEMORY)
            {
                auto memoryInfo = getMemoryInfo();
                
                auto delta1 = memory::compare(record.memoryInfo[0], record.memoryInfo[1]);
                auto delta2 = memory::compare(record.memoryInfo[1], memoryInfo);
                
                memoryStats << " | " <<
                format::bytes(memoryUsage) << ", " <<
                format::bytes(delta1) << ", " <<
                format::bytes(delta2) << " " <<
                memoryInfo;
            }
            
            // ---
            
            LOGI_IF(TextureManager::LOG_VERBOSE) <<
            "TEXTURE UPLOADED: " <<
            request.inputSource->getFilePathHint() << " | " <<
            uniqueId << " | " <<
            width << "x" << height <<
            memoryStats.str() <<
            endl;
        }
    }
    
    void Texture::resetTarget()
    {
        if (target)
        {
            MemoryInfo memoryInfo1;
            
            if (TextureManager::PROBE_MEMORY)
            {
                memoryInfo1 = getMemoryInfo();
            }

            delete target;
            target = nullptr;
            
            glId = 0;
            
            // ---
            
            stringstream memoryStats;
            
            if (TextureManager::PROBE_MEMORY)
            {
                auto memoryInfo2 = getMemoryInfo();
                auto delta = memory::compare(memoryInfo1, memoryInfo2);
                
                memoryStats << " | " <<
                format::bytes(memoryUsage) << ", " <<
                format::bytes(delta) << " " <<
                memoryInfo2;
            }
            
            LOGI_IF(TextureManager::LOG_VERBOSE) <<
            "TEXTURE DISCARDED: " <<
            uniqueId <<
            memoryStats.str() <<
            endl;
        }
    }
    
    bool Texture::bind()
    {
        if (reload())
        {
            glBindTexture(GL_TEXTURE_2D, glId);
        }

        return glId;
    }
    
    void Texture::begin()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        
        bind();
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
        
        const float vertices[] =
        {
            x1, y1,
            x2, y1,
            x2, y2,
            x1, y2
        };
        
        const float coords[] =
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
        const float vertices[] =
        {
            rect.x1, rect.y1,
            rect.x2, rect.y1,
            rect.x2, rect.y2,
            rect.x1, rect.y2
        };
        
        float u1 = (rect.x1 - ox) / width;
        float v1 = (rect.y1 - oy) / height;
        float u2 = (rect.x2 - ox) / width;
        float v2 = (rect.y2 - oy) / height;
        
        const float coords[] =
        {
            u1, v1,
            u2, v1,
            u2, v2,
            u1, v2
        };
        
        glTexCoordPointer(2, GL_FLOAT, 0, coords);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}
