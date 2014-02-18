/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/font/GlyphQuad.h"

#include "cinder/gl/gl.h"

namespace chronotext
{
    struct Vertex2D
    {
        float x;
        float y;
        
        float u;
        float v;
        
        Vertex2D(float x, float y, float u, float v)
        :
        x(x),
        y(y),
        u(u),
        v(v)
        {}
    };

    class GlyphSequence2D
    {
    public:
        static const int stride = sizeof(float) * (2 + 2);

        std::vector<Vertex2D> vertices;
        std::vector<ci::ColorA> colors;
        
        void clear()
        {
            vertices.clear();
            colors.clear();
        }
        
        inline void addQuad(const GlyphQuad &quad)
        {
            vertices.emplace_back(quad.x1, quad.y1, quad.u1, quad.v1);
            vertices.emplace_back(quad.x2, quad.y1, quad.u2, quad.v1);
            vertices.emplace_back(quad.x2, quad.y2, quad.u2, quad.v2);
            vertices.emplace_back(quad.x1, quad.y2, quad.u1, quad.v2);
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
            
            glVertexPointer(2, GL_FLOAT, stride, pointer);
            glTexCoordPointer(2, GL_FLOAT, stride, pointer + 2);
            glColorPointer(4, GL_FLOAT, 0, colors.data());
            
            glDrawElements(GL_TRIANGLES, 6 * size, GL_UNSIGNED_SHORT, indices.data());
        }
    };
}

namespace chr = chronotext;
