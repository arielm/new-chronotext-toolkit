/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/path/FollowablePath.h"
#include "chronotext/utils/stroke/TexturedTriangleStrip.h"

namespace chronotext
{
    class StrokeHelper
    {
    public:
        static void stroke(FollowablePath *path, TexturedTriangleStrip &strip, float width, float ratio = 1)
        {
            auto size = path->size;

            strip.clear();
            strip.vertices.reserve(size * 2);
            strip.coords.reserve(size * 2);
            
            float ufreq = ratio * 0.5f / width;
            
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
                
                float len = path->len[i + o1] - path->len[i + o2];
                float dx = (path->x[i + o1] - path->x[i + o2]) / len;
                float dy = (path->y[i + o1] - path->y[i + o2]) / len;
                float textureU = path->len[i] * ufreq;

                strip.vertices.emplace_back(path->x[i] + width * dy, path->y[i] - width * dx);
                strip.vertices.emplace_back(path->x[i] - width * dy, path->y[i] + width * dx);
                
                strip.coords.emplace_back(textureU, 0);
                strip.coords.emplace_back(textureU, 1);
            }
        }
        
        static void stroke(const std::vector<ci::Vec2f> &points, TexturedTriangleStrip &strip, float width, float ratio = 1)
        {
            auto size = points.size();

            strip.clear();
            strip.vertices.reserve(size * 2);
            strip.coords.reserve(size * 2);

            if (size > 1)
            {
                float textureU = 0;
                float ufreq = ratio * 0.5f / width;
                
                auto p0 = points[0];
                auto p1 = points[1];
                
                if (p0 != p1)
                {
                    float dx = p1.y - p0.y;
                    float dy = p0.x - p1.x;
                    
                    float d = ci::math<float>::sqrt(dx * dx + dy * dy);
                    dx /= d;
                    dy /= d;

                    strip.coords.emplace_back(textureU, 0);
                    strip.coords.emplace_back(textureU, 1);
                    
                    strip.vertices.emplace_back(p0.x - width * dx, p0.y - width * dy);
                    strip.vertices.emplace_back(p0.x + width * dx, p0.y + width * dy);
                    
                    textureU += ufreq * d;
                }
                
                for (int i = 1; i < size - 1; i++)
                {
                    auto p0 = points[i - 1];
                    auto p1 = points[i];
                    auto p2 = points[i + 1];
                    
                    if (p1 != p2)
                    {
                        float dx = p2.y - p0.y;
                        float dy = p0.x - p2.x;
                        
                        float d = ci::math<float>::sqrt(dx * dx + dy * dy);
                        dx /= d;
                        dy /= d;
                        
                        float dist = ci::math<float>::sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
                        textureU += ufreq * dist;

                        strip.coords.emplace_back(textureU, 0);
                        strip.coords.emplace_back(textureU, 1);
                        
                        strip.vertices.emplace_back(p1.x - width * dx, p1.y - width * dy);
                        strip.vertices.emplace_back(p1.x + width * dx, p1.y + width * dy);
                    }
                }
                
                p0 = points[size - 2];
                p1 = points[size - 1];
                
                if (p0 != p1)
                {
                    float dx = p1.y - p0.y;
                    float dy = p0.x - p1.x;
                    
                    float d = ci::math<float>::sqrt(dx * dx + dy * dy);
                    dx /= d;
                    dy /= d;
                    
                    textureU += ufreq * d;

                    strip.coords.emplace_back(textureU, 0);
                    strip.coords.emplace_back(textureU, 1);
                    
                    strip.vertices.emplace_back(p1.x - width * dx, p1.y - width * dy);
                    strip.vertices.emplace_back(p1.x + width * dx, p1.y + width * dy);
                }
            }
        }
    };
}
