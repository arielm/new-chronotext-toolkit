/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/path/FollowablePath.h"
#include "chronotext/path/TexturedTriangleStrip.h"

namespace chronotext
{
    class StrokeHelper
    {
    public:
        static void stroke(const FollowablePath &path, TexturedTriangleStrip &strip, float width, float uScale = 1, float uOffset = 0)
        {
            auto size = path.size();
            
            strip.clear();
            strip.vertices.reserve(size * 4);
            
            if (size > 1)
            {
                const auto &points = path.getPoints();
                const auto &lengths = path.getLengths();

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
                    
                    float length = lengths[i + o1] - lengths[i + o2];
                    ci::Vec2f delta = (points[i + o1] - points[i + o2]) / length;
                    float u = uFreq * (lengths[i] - uOffset);
                    
                    strip.vertices.emplace_back(points[i] + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(points[i] + w2 * delta.yx());
                    strip.vertices.emplace_back(u, 1);
                }
            }
        }
        
        static void stroke(const FollowablePath &path, float offsetStart, float offsetEnd, TexturedTriangleStrip &strip, float width, float uScale = 1, float uOffset = 0)
        {
            auto size = path.size();
            
            strip.clear();
            strip.vertices.reserve(size * 4);
            
            if (size > 1)
            {
                const auto &points = path.getPoints();
                const auto &lengths = path.getLengths();
                
                float halfWidth = width * 0.5f;
                ci::Vec2f w1(+halfWidth, -halfWidth);
                ci::Vec2f w2(-halfWidth, +halfWidth);
                
                float uFreq = uScale / width;
                
                auto valueStart = path.offset2Value(offsetStart);
                auto valueEnd = path.offset2Value(offsetEnd);
                
                int iStart = valueStart.index + 1;
                int iEnd = valueEnd.index;
                
                float length = lengths[iStart] - valueStart.offset;
                ci::Vec2f delta = (points[iStart] - valueStart.position) / length;
                float u = uFreq * (valueStart.offset - uOffset);
                
                strip.vertices.emplace_back(valueStart.position + w1 * delta.yx());
                strip.vertices.emplace_back(u, 0);
                
                strip.vertices.emplace_back(valueStart.position + w2 * delta.yx());
                strip.vertices.emplace_back(u, 1);
                
                for (int i = iStart; i < iEnd; i++)
                {
                    float length = lengths[i + 1] - lengths[i];
                    ci::Vec2f delta = (points[i + 1] - points[i]) / length;
                    float u = uFreq * (lengths[i] - uOffset);
                    
                    strip.vertices.emplace_back(points[i] + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(points[i] + w2 * delta.yx());
                    strip.vertices.emplace_back(u, 1);
                }

                length = lengths[iEnd] - valueEnd.offset;
                delta = (points[iEnd] - valueEnd.position) / length;
                u = uFreq * (valueEnd.offset - uOffset);
                
                strip.vertices.emplace_back(valueEnd.position + w1 * delta.yx());
                strip.vertices.emplace_back(u, 0);
                
                strip.vertices.emplace_back(valueEnd.position + w2 * delta.yx());
                strip.vertices.emplace_back(u, 1);
            }
        }
        
        static void stroke(const std::vector<ci::Vec2f> &points, TexturedTriangleStrip &strip, float width, float uScale = 1, float uOffset = 0)
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
                float u = -uOffset * uFreq;
                
                auto p0 = points[0];
                auto p1 = points[1];
                
                if (p0 != p1)
                {
                    ci::Vec2f delta(p1 - p0);
                    float length = delta.length();
                    delta /= length;
                    
                    u += uFreq * length;
                    
                    strip.vertices.emplace_back(p0 + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(p0 + w2 * delta.yx());
                    strip.vertices.emplace_back(u, 1);
                }
                
                for (int i = 1; i < size - 1; i++)
                {
                    auto p0 = points[i - 1];
                    auto p1 = points[i];
                    auto p2 = points[i + 1];
                    
                    if (p1 != p2)
                    {
                        ci::Vec2f delta = (p2 - p1).normalized();
                        u += uFreq * (p1 - p0).length();
                        
                        strip.vertices.emplace_back(p1 + w1 * delta.yx());
                        strip.vertices.emplace_back(u, 0);
                        
                        strip.vertices.emplace_back(p1 + w2 * delta.yx());
                        strip.vertices.emplace_back(u, 1);
                    }
                }
                
                p0 = points[size - 2];
                p1 = points[size - 1];
                
                if (p0 != p1)
                {
                    ci::Vec2f delta(p1 - p0);
                    float length = delta.length();
                    delta /= length;
                    
                    u += uFreq * length;
                    
                    strip.vertices.emplace_back(p1 + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(p1 + w2 * delta.yx());
                    strip.vertices.emplace_back(u, 1);
                }
            }
        }
    };
}
