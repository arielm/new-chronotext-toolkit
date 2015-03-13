/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#pragma once

#include "chronotext/quad/Quad.h"

#include "cinder/gl/gl.h"

namespace chr
{
    struct Vertex
    {
        float x;
        float y;
        float z;
        
        float u;
        float v;
        
        Vertex(float x, float y, float z, float u, float v)
        :
        x(x),
        y(y),
        z(z),
        u(u),
        v(v)
        {}
    };
    
    class QuadBatch
    {
    public:
        static constexpr int stride = sizeof(float) * (3 + 2);
        
        std::vector<Vertex> vertices;
        std::vector<ci::ColorA> colors;
        
        void clear()
        {
            vertices.clear();
            colors.clear();
        }
        
        void pack()
        {
            vertices.shrink_to_fit();
            colors.shrink_to_fit();
        }

        int size() const
        {
            return vertices.size() >> 2;
        }
        
        bool empty() const
        {
            return vertices.empty();
        }

        inline void addQuad(const Quad &quad, float z = 0)
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
        
        void flush(const uint16_t *indices, bool useColor = false) const
        {
            const float *pointer = reinterpret_cast<const float*>(vertices.data());

            if (useColor)
            {
                glColorPointer(4, GL_FLOAT, 0, colors.data());
            }

            glVertexPointer(3, GL_FLOAT, stride, pointer);
            glTexCoordPointer(2, GL_FLOAT, stride, pointer + 3);
            glDrawElements(GL_TRIANGLES, 6 * size(), GL_UNSIGNED_SHORT, indices);
        }
    };
}
