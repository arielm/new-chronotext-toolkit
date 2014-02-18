/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/GlyphQuad.h"

#include "cinder/Color.h"

namespace chronotext
{
    struct Vertex3D
    {
        float x;
        float y;
        float z;
        
        float u;
        float v;
        
        Vertex3D(float x, float y, float z, float u, float v)
        :
        x(x),
        y(y),
        z(z),
        u(u),
        v(v)
        {}
    };
    
    class GlyphSequence3D
    {
    public:
        static const int stride = sizeof(float) * (3 + 2);
        
        std::vector<Vertex3D> vertices;
        std::vector<ci::ColorA> colors;
        
        void clear()
        {
            vertices.clear();
            colors.clear();
        }
        
        inline void addQuad(const GlyphQuad &quad, float z = 0)
        {
            vertices.emplace_back(quad.x1, quad.y1, z, quad.u1, quad.v1);
            vertices.emplace_back(quad.x2, quad.y1, z, quad.u2, quad.v1);
            vertices.emplace_back(quad.x2, quad.y2, z, quad.u2, quad.v2);
            vertices.emplace_back(quad.x1, quad.y2, z, quad.u1, quad.v2);
        }
        
        inline void addColor(const ci::ColorA &color)
        {
            colors.emplace_back(color);
            colors.emplace_back(color);
            colors.emplace_back(color);
            colors.emplace_back(color);
        }
        
        void flush(const std::vector<GLushort> &indices)
        {
            float *pointer = reinterpret_cast<float*>(vertices.data());
            int size = vertices.size() >> 2;
            
            glVertexPointer(3, GL_FLOAT, stride, pointer);
            glTexCoordPointer(2, GL_FLOAT, stride, pointer + 3);
            glColorPointer(4, GL_FLOAT, 0, colors.data());
            
            glDrawElements(GL_TRIANGLES, 6 * size, GL_UNSIGNED_SHORT, indices.data());
        }
    };
}

namespace chr = chronotext;
