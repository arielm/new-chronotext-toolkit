#pragma once

#include "chronotext/path/FollowablePath.h"
#include "chronotext/utils/TexturedTriangleStrip.h"

#include <cmath>

class StrokeHelper
{
public:
    static void stroke(FollowablePath *path, TexturedTriangleStrip &strip, float width, float ratio = 1)
    {
        int size = path->size;
        
        float *vertices = new float[size * 4];
        float *coords = new float[size * 4];
        
        strip.count = size * 2;
        strip.vertices = vertices;
        strip.coords = coords;
        
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
            
            *vertices++ = path->x[i] + width * dy;
            *vertices++ = path->y[i] - width * dx;
            
            *vertices++ = path->x[i] - width * dy;
            *vertices++ = path->y[i] + width * dx;
            
            float textureU = path->len[i] * ufreq;

            *coords++ = textureU;
            *coords++ = 0;
            *coords++ = textureU;
            *coords++ = 1;
        }
    }

    static void stroke(const std::vector<ci::Vec2f> &points, TexturedTriangleStrip &strip, float width, float ratio = 1)
    {
        int size = points.size();
        strip.count = 0;
        
        if (size > 1)
        {
            float *vertices = new float[size * 4];
            float *coords = new float[size * 4];
            
            strip.vertices = vertices;
            strip.coords = coords;
            
            // ---
            
            ci::Vec2f p0, p1, p2;
            float dx, dy;
            float d;
            
            float textureU = 0;
            float ufreq = ratio * 0.5f / width;
            
            // ---
            
            p0 = points[0];
            p1 = points[1];

            if (p0 != p1)
            {
                strip.count += 2;
                
                dx = p1.y - p0.y;
                dy = p0.x - p1.x;
                
                d = sqrt(dx * dx + dy * dy);
                dx /= d;
                dy /= d;
                
                *coords++ = textureU;
                *coords++ = 0;
                *coords++ = textureU;
                *coords++ = 1;
                
                *vertices++ = p0.x - width * dx;
                *vertices++ = p0.y - width * dy;
                *vertices++ = p0.x + width * dx;
                *vertices++ = p0.y + width * dy;
                
                textureU += ufreq * d;
            }
            
            for (int i = 1; i < size - 1; i++)
            {
                p0 = points[i - 1];
                p1 = points[i];
                p2 = points[i + 1];

                if (p1 != p2)
                {
                    strip.count += 2;

                    dx = p2.y - p0.y;
                    dy = p0.x - p2.x;
                    
                    d = sqrt(dx * dx + dy * dy);
                    dx /= d;
                    dy /= d;
                    
                    float dist = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y));
                    textureU += ufreq * dist;
                    
                    *coords++ = textureU;
                    *coords++ = 0;
                    *coords++ = textureU;
                    *coords++ = 1;
                    
                    *vertices++ = p1.x - width * dx;
                    *vertices++ = p1.y - width * dy;
                    *vertices++ = p1.x + width * dx;
                    *vertices++ = p1.y + width * dy;
                }
            }
            
            p0 = points[size - 2];
            p1 = points[size - 1];
            
            if (p0 != p1)
            {
                strip.count += 2;
                
                dx = p1.y - p0.y;
                dy = p0.x - p1.x;
                
                d = sqrt(dx * dx + dy * dy);
                dx /= d;
                dy /= d;
                
                textureU += ufreq * d;
                
                *coords++ = textureU;
                *coords++ = 0;
                *coords++ = textureU;
                *coords   = 1;
                
                *vertices++ = p1.x - width * dx;
                *vertices++ = p1.y - width * dy;
                *vertices++ = p1.x + width * dx;
                *vertices   = p1.y + width * dy;
            }
        }
    }
};
