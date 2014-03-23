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
#include "chronotext/utils/Utils.h"

namespace chronotext
{
    class StrokeHelper
    {
    public:
        /*
         * width: WIDTH OF THE STROKE (MAPPED TO THE HEIGHT OF THE TEXTURE)
         * uScale: RATIO BETWEEN THE HEIGHT AND WIDTH OF THE TEXTURE
         * uOffset: HORIZONTAL SHIFT OF THE TEXTURE, IN PIXELS
         *
         * CREATES A SINGLE-PORTION STRIP:
         * THE TexturedTriangleStrip IS AUTOMATICALLY CLEARED
         */
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
        
        /*
         * offsetBegin: DISTANCE BETWEEN THE BEGINNING OF THE PATH AND THE BEGINNING OF THE STROKE, IN PIXELS
         * offsetEnd: DISTANCE BETWEEN THE BEGINNING OF THE PATH AND THE END OF THE STROKE, IN PIXELS
         * width: WIDTH OF THE STROKE (MAPPED TO THE HEIGHT OF THE TEXTURE)
         * uScale: RATIO BETWEEN THE HEIGHT AND WIDTH OF THE TEXTURE
         * uOffset: HORIZONTAL SHIFT OF THE TEXTURE, IN PIXELS
         * discontinuous: IF TRUE, THE STRIP WILL BE DEGENERATED (I.E. ZERO-AREA TRIANGLES WILL BE INSERTED BETWEEN PORTIONS)
         *
         * POSSIBILITY TO CREATE A MULTI-PORTION STRIP:
         * THE TexturedTriangleStrip SHOULD THEREFORE BE CLEARED MANUALLY
         */
        static int stroke(const FollowablePath &path, float offsetBegin, float offsetEnd, TexturedTriangleStrip &strip, float width, float uScale = 1, float uOffset = 0, bool discontinuous = true)
        {
            int originalSize = strip.vertices.size();
            
            auto valueBegin = offset2Value(path, offsetBegin);
            auto valueEnd = offset2Value(path, offsetEnd);
            
            if (valueEnd.offset > valueBegin.offset)
            {
                const auto &points = path.getPoints();
                const auto &lengths = path.getLengths();
                
                float halfWidth = width * 0.5f;
                ci::Vec2f w1(+halfWidth, -halfWidth);
                ci::Vec2f w2(-halfWidth, +halfWidth);
                
                float uFreq = uScale / width;
                
                float length = lengths[valueBegin.index + 1] - valueBegin.offset;
                ci::Vec2f delta = (points[valueBegin.index + 1] - valueBegin.position) / length;
                float u = uFreq * (valueBegin.offset - uOffset);
                
                if (discontinuous && !strip.empty())
                {
                    strip.vertices.emplace_back(strip.vertices[strip.vertices.size() - 2]);
                    strip.vertices.emplace_back(0, 0);
                }
                
                for (int j = 0; j < ((discontinuous && !strip.empty()) ? 2 : 1); j++)
                {
                    strip.vertices.emplace_back(valueBegin.position + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                }
                
                strip.vertices.emplace_back(valueBegin.position + w2 * delta.yx());
                strip.vertices.emplace_back(u, 1);
                
                for (int i = valueBegin.index + 1; i < valueEnd.index; i++)
                {
                    float length = lengths[i + 1] - lengths[i];
                    ci::Vec2f delta = (points[i + 1] - points[i]) / length;
                    float u = uFreq * (lengths[i] - uOffset);
                    
                    strip.vertices.emplace_back(points[i] + w1 * delta.yx());
                    strip.vertices.emplace_back(u, 0);
                    
                    strip.vertices.emplace_back(points[i] + w2 * delta.yx());
                    strip.vertices.emplace_back(u, 1);
                }

                length = lengths[valueEnd.index] - valueEnd.offset;
                delta = (points[valueEnd.index] - valueEnd.position) / length;
                u = uFreq * (valueEnd.offset - uOffset);
                
                strip.vertices.emplace_back(valueEnd.position + w1 * delta.yx());
                strip.vertices.emplace_back(u, 0);
                
                strip.vertices.emplace_back(valueEnd.position + w2 * delta.yx());
                strip.vertices.emplace_back(u, 1);
            }
            
            return (strip.vertices.size() - originalSize) >> 1;
        }
        
        /*
         * width: WIDTH OF THE STROKE (MAPPED TO THE HEIGHT OF THE TEXTURE)
         * uScale: RATIO BETWEEN THE HEIGHT AND WIDTH OF THE TEXTURE
         * uOffset: HORIZONTAL SHIFT OF THE TEXTURE, IN PIXELS
         *
         * CREATES A SINGLE-PORTION STRIP:
         * THE TexturedTriangleStrip IS AUTOMATICALLY CLEARED
         */
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
        
    protected:
        static FollowablePath::Value offset2Value(const FollowablePath &path, float offset)
        {
            const auto &points = path.getPoints();
            const auto &lengths = path.getLengths();
            float length = path.getLength();
            
            if (offset < 0)
            {
                offset = 0;
            }
            else if (offset > length)
            {
                offset = length;
            }
            
            int index = search(lengths, offset, 1, lengths.size());
            auto &p0 = points[index];
            auto &p1 = points[index + 1];
            
            float ratio = (offset - lengths[index]) / (lengths[index + 1] - lengths[index]);
            
            FollowablePath::Value value;
            value.position = p0 + (p1 - p0) * ratio;
            value.offset = offset;
            value.index = index;
            
            return value;
        }
    };
}
