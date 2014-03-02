/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/path/FollowablePath.h"
#include "chronotext/path/TexturedTriangleStrip.h"

/*
 * ASSOCIATED TEXTURES SHOULD HAVE THEIR "GL_TEXTURE_WRAP_S" SET TO "GL_REPEAT",
 */

namespace chronotext
{
    class StrokeHelper
    {
    public:
        static void stroke(const FollowablePath &path, TexturedTriangleStrip &strip, float width, float uScale = 1)
        {
            auto size = path.size();
            const auto &points = path.getPoints();
            const auto &lengths = path.getLengths();
            
            strip.clear();
            strip.vertices.reserve(size * 4);
            
            float halfWidth = width * 0.5f;
            ci::Vec2f w1(+halfWidth, -halfWidth);
            ci::Vec2f w2(-halfWidth, +halfWidth);

            float uFreq = uScale / width;

            for (int i = 0; i < size; i++)
            {
                int o1, o2;
                
                if (i == size - 1)
                {
                    o1 = 0;
                    o2 = -1;
                }
                else
                {
                    o1 = 1;
                    o2 = 0;
                }
                
                float l = lengths[i + o1] - lengths[i + o2];
                ci::Vec2f d = (points[i + o1] - points[i + o2]) / l;
                float u = uFreq * lengths[i];
                
                strip.vertices.emplace_back(points[i] + w1 * d.yx());
                strip.vertices.emplace_back(u, 0);
                
                strip.vertices.emplace_back(points[i] + w2 * d.yx());
                strip.vertices.emplace_back(u, 1);
            }
        }
        
        static void stroke(const std::vector<ci::Vec2f> &points, TexturedTriangleStrip &strip, float width, float uScale = 1)
        {
            auto size = points.size();
            
            strip.clear();
            strip.vertices.reserve(size * 4);
            
            if (size > 1)
            {
                float halfWidth = width * 0.5f;
                ci::Vec2f w1(+halfWidth, -halfWidth);
                ci::Vec2f w2(-halfWidth, +halfWidth);

                float uFreq = uScale / width;
                float u = 0;

                auto p0 = points[0];
                auto p1 = points[1];
                
                if (p0 != p1)
                {
                    ci::Vec2f d(p1 - p0);
                    float l = d.length();
                    d /= l;

                    u += uFreq * l;

                    strip.vertices.emplace_back(p0 + w1 * d.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(p0 + w2 * d.yx());
                    strip.vertices.emplace_back(u, 1);
                }
                
                for (int i = 1; i < size - 1; i++)
                {
                    auto p0 = points[i - 1];
                    auto p1 = points[i];
                    auto p2 = points[i + 1];
                    
                    if (p1 != p2)
                    {
                        ci::Vec2f d = (p2 - p1).normalized();
                        u += uFreq * (p1 - p0).length();
                        
                        strip.vertices.emplace_back(p1 + w1 * d.yx());
                        strip.vertices.emplace_back(u, 0);
                        
                        strip.vertices.emplace_back(p1 + w2 * d.yx());
                        strip.vertices.emplace_back(u, 1);
                    }
                }
                
                p0 = points[size - 2];
                p1 = points[size - 1];
                
                if (p0 != p1)
                {
                    ci::Vec2f d(p1 - p0);
                    float l = d.length();
                    d /= l;
                    
                    u += uFreq * l;
                    
                    strip.vertices.emplace_back(p1 + w1 * d.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(p1 + w2 * d.yx());
                    strip.vertices.emplace_back(u, 1);
                }
            }
        }
    };
}
